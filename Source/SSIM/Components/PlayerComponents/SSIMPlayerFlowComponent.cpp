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
	PlayerCombatComponent->OnAttackStartedDelegate.AddDynamic(this, &USSIMPlayerFlowComponent::OnAttackStartedHandler);
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
	
	// Should implement OnCompleted/OnBlendOut/AnimNotify bDashing reset
	FTimerHandle DashInProcessTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
									DashInProcessTimerHandle, 
									this, 
									&USSIMPlayerFlowComponent::EndDash, 
									PlayerDashMontage->GetPlayLength(), 
									false);
	
	SSIMPlayer->GetCharacterMovement()->BrakingDecelerationWalking = DashBrakingDecelerationWalking;
	SSIMPlayer->SetContactDamageCollisionShapeDash();
	SSIMPlayer->GetCharacterMovement()->GravityScale = DashGravityScale;
	
	SSIMPlayer->GetCharacterMovement()->BrakingDecelerationFalling = DashBrakingDecelerationFalling;
	
	if (SSIMPlayer->GetCharacterMovement()->IsFalling())
	{
		SSIMPlayer->LandedDelegate.AddDynamic(this, &USSIMPlayerFlowComponent::ResetDashFromAir);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(
			DashCooldownTimerHandle, 
			this, 
			&USSIMPlayerFlowComponent::ResetDash, 
			DashCooldown, 
			false);
	}
	
	OnDashStartedDelegate.Broadcast();
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
	
	FVector OutLaunchVelocity =  DashDirectionVector *
								 SSIMPlayer->GetCharacterMovement()->GetMaxSpeed() *
								 DashVelocityCoef;
	
	if (bShowDashLogs)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Dash Launch Velocity: %s"), TEXT(__FUNCTION__), *OutLaunchVelocity.ToString());
	}
	
	return OutLaunchVelocity;
}

void USSIMPlayerFlowComponent::EndDash()
{
	OnDashEndedDelegate.Broadcast();
	SSIMPlayer->SetPlayerBrakingDecelerationWalkingToDefault();
	SSIMPlayer->SetContactDamageCollisionShapeDefault();
	SSIMPlayer->SetPlayerGravityScaleToDefault();
	
	SSIMPlayer->GetCharacterMovement()->BrakingDecelerationFalling = 0.f;
	
	if (bShowDashLogs)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Dash ENDED"), TEXT(__FUNCTION__));
	}
	
	/*if (SSIMPlayer->GetCharacterMovement()->IsFalling())
	{
		SSIMPlayer->LandedDelegate.AddUniqueDynamic(this, &USSIMPlayerFlowComponent::ResetBrakingDecelerationFalling);
		SSIMPlayer->GetCharacterMovement()->BrakingDecelerationFalling = DashBrakingDecelerationFalling;
	}*/
}

void USSIMPlayerFlowComponent::ResetDash()
{
	OnCanDashChangedDelegate.Broadcast(true);
}

void USSIMPlayerFlowComponent::ResetDashFromAir(const FHitResult& Hit)
{
	SSIMPlayer->LandedDelegate.RemoveDynamic(this, &USSIMPlayerFlowComponent::ResetDashFromAir);
	
	OnCanDashChangedDelegate.Broadcast(true);
}

void USSIMPlayerFlowComponent::ResetBrakingDecelerationFalling(const FHitResult& Hit)
{
	/*SSIMPlayer->GetCharacterMovement()->BrakingDecelerationFalling = 0.f;
	SSIMPlayer->LandedDelegate.RemoveDynamic(this, &USSIMPlayerFlowComponent::ResetBrakingDecelerationFalling);*/
}

void USSIMPlayerFlowComponent::OnDamageReceivedHandler(const FDamageData& DamageData)
{
	EndDash();
}

void USSIMPlayerFlowComponent::OnAttackStartedHandler()
{
	
}
