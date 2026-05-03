// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerDashComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "SSIM/SSIM.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "SSIM/Characters/Player/SSIMPlayer.h"
#include "SSIM/Components/Combat/SSIMPlayerCombatComponent.h"
#include "SSIM/Components/Stats/SSIMPlayerStatsComponent.h"


// Overriden Functions
void USSIMPlayerDashComponent::BeginPlay()
{
	Super::BeginPlay();
	SetReferences();
	
	if (!IsValid(PlayerStatsComponent))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | Stats component is not valid"), TEXT(__FUNCTION__));
	}
	PlayerStatsComponent->OnDamageReceivedDelegate.AddDynamic(this, &USSIMPlayerDashComponent::OnDamageReceivedHandler);
	PlayerCombatComponent->OnPogoStartedDelegate.AddUniqueDynamic(this, &USSIMPlayerDashComponent::ResetDashOnPogo);
	
}

void USSIMPlayerDashComponent::SetReferences()
{
	Super::SetReferences();
	
	SSIMPlayer = CastChecked<ASSIMPlayer>(SSIMOwnerCharacter);
	PlayerStatsComponent  = SSIMPlayer->GetPlayerStatsComponent();
	PlayerCombatComponent = SSIMPlayer->GetPlayerCombatComponent();
}


// My Functions
void USSIMPlayerDashComponent::StartDash()
{	
	if (!IsValid(PlayerDashMontage))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | PlayerDashAnimation is not valid"), TEXT(__FUNCTION__));
		return;
	}
	AnimInstance->Montage_Play(PlayerDashMontage);
	
	SSIMPlayer->StopJumping();
	SSIMPlayer->LaunchCharacter(GetDashLaunchVelocity() ,true, true);
	//SSIMPlayer->SetContactDamageCollisionShapeDash();
	SSIMPlayer->GetCharacterMovement()->GravityScale = DashGravityScale;
	
	SetDashDamageCollision();
	
	FTimerHandle DashInProcessTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
									DashInProcessTimerHandle, 
									this, 
									&USSIMPlayerDashComponent::EndDash, 
									DashDuration, 
									false);
	
	
	if (SSIMPlayer->GetCharacterMovement()->IsFalling())
	{
		SSIMPlayer->LandedDelegate.AddUniqueDynamic(this, &USSIMPlayerDashComponent::OnLandedFromDashHandler);
		SSIMPlayer->GetCharacterMovement()->BrakingDecelerationFalling = DashBrakingDecelerationFalling;
	}
	else
	{
		SSIMPlayer->GetCharacterMovement()->BrakingDecelerationWalking = DashBrakingDecelerationWalking;
		
		GetWorld()->GetTimerManager().SetTimer(
			DashCooldownTimerHandle, 
			this, 
			&USSIMPlayerDashComponent::ResetDash, 
			DashCooldown, 
			false);
	}
	
	bDashing = true;
	OnDashStartedDelegate.Broadcast();
	bCanDash = false;
	OnCanDashChangedDelegate.Broadcast(false);
	
	if (bShowDashLogs)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Dash STARTED"), TEXT(__FUNCTION__));
	}
}

void USSIMPlayerDashComponent::EndDash()
{
	bDashing = false;
	OnDashEndedDelegate.Broadcast();
	
	//SSIMPlayer->SetContactDamageCollisionShapeDefault();
	SSIMPlayer->SetPlayerBrakingDecelerationWalkingToDefault();
	SSIMPlayer->GetCharacterMovement()->BrakingDecelerationFalling = 0.f;
	SSIMPlayer->SetPlayerGravityScaleToDefault();
	
	SetDefaultDamageCollision();
	
	SSIMPlayer->GetCharacterMovement()->StopMovementImmediately();
	SSIMPlayer->GetMesh()->GetAnimInstance()->Montage_Stop(PlayerDashMontage->BlendOut.GetBlendTime());
	
	if (bShowDashLogs)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Dash ENDED"), TEXT(__FUNCTION__));
	}
}

void USSIMPlayerDashComponent::ResetDash()
{
	if (bShowDashLogs)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Dash RESET"), TEXT(__FUNCTION__));
	}
	
	bCanDash = true;
	OnCanDashChangedDelegate.Broadcast(true);
}

void USSIMPlayerDashComponent::ResetDashFromAir()
{
	if (bShowDashLogs)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Dash From Air RESET"), TEXT(__FUNCTION__));
	}
	
	bCanDash = true;
	OnCanDashChangedDelegate.Broadcast(true);
}

void USSIMPlayerDashComponent::ResetDashOnPogo()
{
	if (!SSIMPlayer->GetCanPlayerDash())
	{
		if (bShowDashLogs)
		{
			UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Dash on Pogo RESET"), TEXT(__FUNCTION__));
		}
		bCanDash = true;
		OnCanDashChangedDelegate.Broadcast(true);
	}
}

FVector USSIMPlayerDashComponent::GetDashLaunchVelocity() const
{
	// Calculate Player direction
	FVector DashDirectionVector;
	float DirectionDotProduct = FVector::DotProduct(SSIMPlayer->GetActorForwardVector(), FVector::RightVector);
		
	if (FMath::IsNearlyEqual(DirectionDotProduct, 1.f))
	{
		DashDirectionVector = FVector::RightVector;
	}
	else if (FMath::IsNearlyEqual(DirectionDotProduct, -1.f))
	{
		DashDirectionVector = FVector::RightVector * -1.f;
	}
	else
	{
		UE_LOG(LogSSIMGameplayMessages, Warning, TEXT("%s | Couldn't determine player direction. Return -1.f"), TEXT(__FUNCTION__));
		return FVector(-1.f, -1.f, -1.f);
	}
	
	FVector OutLaunchVelocity =  DashDirectionVector * DashVelocity;
	
	if (bShowDashLogs)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Dash Launch Velocity: %s"), TEXT(__FUNCTION__), *OutLaunchVelocity.ToString());
	}
	
	return OutLaunchVelocity;
}

void USSIMPlayerDashComponent::SetDashDamageCollision() const
{
	SSIMPlayer->GetHitRegistrationCollision()->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	SSIMPlayer->GetContactDamageCollision()->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	SSIMPlayer->GetHitRegistrationCollision()->SetCapsuleHalfHeight(DashHitRegistrationCollisionHalfHeight);
	SSIMPlayer->GetContactDamageCollision()->SetCapsuleHalfHeight(DashContactDamageCollisionHalfHeight);
}
void USSIMPlayerDashComponent::SetDefaultDamageCollision() const
{
	SSIMPlayer->GetContactDamageCollision()->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	SSIMPlayer->GetHitRegistrationCollision()->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	SSIMPlayer->GetHitRegistrationCollision()->SetCapsuleHalfHeight(SSIMPlayer->DefaultContactDamageCollisionRadius);
	SSIMPlayer->GetContactDamageCollision()->SetCapsuleHalfHeight(SSIMPlayer->DefaultHitRegistrationCollisionHalfHeight);
}

void USSIMPlayerDashComponent::OnLandedFromDashHandler(const FHitResult& Hit)
{
	ResetDashFromAir();
	AnimInstance->Montage_Stop(0.2f,PlayerDashMontage);
	SSIMPlayer->LandedDelegate.RemoveDynamic(this, &USSIMPlayerDashComponent::OnLandedFromDashHandler);
}

void USSIMPlayerDashComponent::OnDamageReceivedHandler(const FDamageData& DamageData)
{
	EndDash();
}
