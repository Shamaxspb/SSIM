// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerFlowComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "SSIM/SSIM.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "SSIM/Characters/Player/SSIMPlayer.h"
#include "SSIM/Components/Combat/SSIMPlayerCombatComponent.h"
#include "SSIM/Components/Stats/SSIMPlayerStatsComponent.h"


// Overriden Functions
void USSIMPlayerFlowComponent::BeginPlay()
{
	Super::BeginPlay();
	SetReferences();
	
	if (!IsValid(PlayerStatsComponent))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | Stats component is not valid"), TEXT(__FUNCTION__));
	}
	PlayerStatsComponent->OnDamageReceivedDelegate.AddDynamic(this, &USSIMPlayerFlowComponent::OnDamageReceivedHandler);
	PlayerCombatComponent->OnPogoStartedDelegate.AddUniqueDynamic(this, &USSIMPlayerFlowComponent::ResetDashOnPogo);
	
}

void USSIMPlayerFlowComponent::SetReferences()
{
	Super::SetReferences();
	
	SSIMPlayer = CastChecked<ASSIMPlayer>(SSIMOwnerCharacter);
	PlayerStatsComponent  = SSIMPlayer->GetPlayerStatsComponent();
	PlayerCombatComponent = SSIMPlayer->GetPlayerCombatComponent();
}


// My Functions
void USSIMPlayerFlowComponent::StartDash()
{	
	if (!IsValid(PlayerDashMontage))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | PlayerDashAnimation is not valid"), TEXT(__FUNCTION__));
		return;
	}
	AnimInstance->Montage_Play(PlayerDashMontage);
	
	SSIMPlayer->StopJumping();
	SSIMPlayer->LaunchCharacter(GetDashLaunchVelocity() ,true, true);
	SSIMPlayer->SetContactDamageCollisionShapeDash();
	SSIMPlayer->GetCharacterMovement()->GravityScale = DashGravityScale;
	
	
	FTimerHandle DashInProcessTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
									DashInProcessTimerHandle, 
									this, 
									&USSIMPlayerFlowComponent::EndDash, 
									DashDuration, 
									false);
	
	
	if (SSIMPlayer->GetCharacterMovement()->IsFalling())
	{
		SSIMPlayer->LandedDelegate.AddUniqueDynamic(this, &USSIMPlayerFlowComponent::ResetDashFromAir);
		SSIMPlayer->GetCharacterMovement()->BrakingDecelerationFalling = DashBrakingDecelerationFalling;
	}
	else
	{
		SSIMPlayer->GetCharacterMovement()->BrakingDecelerationWalking = DashBrakingDecelerationWalking;
		
		GetWorld()->GetTimerManager().SetTimer(
			DashCooldownTimerHandle, 
			this, 
			&USSIMPlayerFlowComponent::ResetDash, 
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

FVector USSIMPlayerFlowComponent::GetDashLaunchVelocity() const
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

void USSIMPlayerFlowComponent::EndDash()
{
	bDashing = false;
	OnDashEndedDelegate.Broadcast();
	
	SSIMPlayer->SetContactDamageCollisionShapeDefault();
	SSIMPlayer->SetPlayerBrakingDecelerationWalkingToDefault();
	SSIMPlayer->GetCharacterMovement()->BrakingDecelerationFalling = 0.f;
	SSIMPlayer->SetPlayerGravityScaleToDefault();
	
	SSIMPlayer->GetCharacterMovement()->StopMovementImmediately();
	// SSIMPlayer->GetMesh()->GetAnimInstance()->Montage_Stop(0.09f, PlayerDashMontage);
	SSIMPlayer->GetMesh()->GetAnimInstance()->Montage_Stop(PlayerDashMontage->BlendOut.GetBlendTime());
	
	if (bShowDashLogs)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Dash ENDED"), TEXT(__FUNCTION__));
	}
}

void USSIMPlayerFlowComponent::ResetDash()
{
	if (bShowDashLogs)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Dash RESET"), TEXT(__FUNCTION__));
	}
	
	bCanDash = true;
	OnCanDashChangedDelegate.Broadcast(true);
}

void USSIMPlayerFlowComponent::ResetDashFromAir(const FHitResult& Hit)
{
	if (bShowDashLogs)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Dash From Air RESET"), TEXT(__FUNCTION__));
	}
	
	SSIMPlayer->LandedDelegate.RemoveDynamic(this, &USSIMPlayerFlowComponent::ResetDashFromAir);
	
	bCanDash = true;
	OnCanDashChangedDelegate.Broadcast(true);
}

void USSIMPlayerFlowComponent::ResetDashOnPogo()
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



void USSIMPlayerFlowComponent::OnDamageReceivedHandler(const FDamageData& DamageData)
{
	EndDash();
}
