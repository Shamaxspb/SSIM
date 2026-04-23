// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerFlowComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "SSIM/SSIM.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "SSIM/Components/Stats/SSIMPlayerStatsComponent.h"


// Overriden Functions
void USSIMPlayerFlowComponent::BeginPlay()
{
	Super::BeginPlay();
	
	StatsComponent = SSIMOwnerCharacter->FindComponentByClass<USSIMPlayerStatsComponent>();
	
	
	if (!IsValid(StatsComponent))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | Stats component is not valid"), TEXT(__FUNCTION__));
	}
	StatsComponent->OnDamageReceivedDelegate.AddDynamic(this, &USSIMPlayerFlowComponent::OnDamageReceivedHandler);
	
}


// My Functions
void USSIMPlayerFlowComponent::StartDash()
{
	if (bDashing || !bCanDash || StatsComponent->bStaggered)
	{
		if (bDashing)
		{
			UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Dash is still in process"), TEXT(__FUNCTION__));
		}
		if (!bCanDash)
		{
			UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Dash is on cooldown for %f"), TEXT(__FUNCTION__), GetWorld()->GetTimerManager().GetTimerRemaining(DashCooldownTimerHandle));
			return;	
		}
		if (StatsComponent->bStaggered)
		{
			UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Can't Dash during stagger"), TEXT(__FUNCTION__));
			return;	
		}
	}
	
	bDashing = true;
	bCanDash = false;	
	
	SSIMOwnerCharacter->LaunchCharacter(GetDashLaunchVelocity() ,true, false);

	if (!IsValid(PlayerDashAnimation))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | PlayerDashAnimation is not valid"), TEXT(__FUNCTION__));
		return;
	}
	AnimInstance->Montage_Play(PlayerDashAnimation);
	
	// Should implement OnCompleted/OnBlendOut/AnimNotify bDashing reset
	FTimerHandle DashInProcessTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DashInProcessTimerHandle, this, &USSIMPlayerFlowComponent::EndDash, PlayerDashAnimation->GetPlayLength(), false);
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | PlayerDash animation length: %f"), TEXT(__FUNCTION__), PlayerDashAnimation->GetPlayLength());
	
	GetWorld()->GetTimerManager().SetTimer(DashCooldownTimerHandle, this, &USSIMPlayerFlowComponent::ResetDash, DashCooldown, false);
	
	OnStartDashDelegate.Broadcast();
	
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Dash started"), TEXT(__FUNCTION__));
}

FVector USSIMPlayerFlowComponent::GetDashLaunchVelocity() const
{
	FVector CurrentVelocity = SSIMOwnerCharacter->GetVelocity();
	FVector OutLaunchVelocity;
	
	if (CurrentVelocity.IsNearlyZero())
	{
		// Dash in place
		
		// Calculate Player direction
		FVector DashDirectionVector;
		float DirectionDotProduct = FVector::DotProduct(SSIMOwnerCharacter->GetActorForwardVector(), FVector::RightVector);
		
		
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
		
		OutLaunchVelocity =  DashDirectionVector *
							 SSIMOwnerCharacter->GetCharacterMovement()->GetMaxSpeed() *
							 DashVelocityCoef;
		
	}
	else
	{
		// Dash in motion
		OutLaunchVelocity = FVector(0.f, SSIMOwnerCharacter->GetVelocity().Y * DashVelocityCoef,0.f);
	}
		
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Dash Launch Velocity: %s"), TEXT(__FUNCTION__), *OutLaunchVelocity.ToString());
	return OutLaunchVelocity;
}

void USSIMPlayerFlowComponent::EndDash()
{
	bDashing = false;
	
	OnEndDashDelegate.Broadcast();
}

void USSIMPlayerFlowComponent::ResetDash()
{
	bCanDash = true;
}

void USSIMPlayerFlowComponent::OnDamageReceivedHandler(const FDamageData DamageData)
{
	EndDash();
}


// DEBUG
void USSIMPlayerFlowComponent::TakeDamageFromNearestEnemy()
{
	
}
