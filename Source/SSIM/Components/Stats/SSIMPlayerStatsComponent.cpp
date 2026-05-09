// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerStatsComponent.h"

#include "SSIM/SSIM.h"
#include "SSIM/Characters/Player/SSIMPlayer.h"
#include "SSIM/Core/Types/SSIMCombatDataTypes.h"


// My Functions
void USSIMPlayerStatsComponent::SetReferences()
{
	Super::SetReferences();
	
	SSIMPlayer = CastChecked<ASSIMPlayer>(SSIMOwnerCharacter);
}

void USSIMPlayerStatsComponent::ReduceHealth(const FDamageData& InDamageData)
{
	if (Health - InDamageData.Value <= 0)
	{
		UE_LOG(LogSSIMStatsCalculation, Warning, TEXT("%s | Player killed by %s"), TEXT(__FUNCTION__), *InDamageData.Instigator->GetName());
	}
	
	if (bInvulnerable)
	{
		if (bShowStatsLogs)
		{
			if (bShowInvulnerabilityCheck)
			{
				UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Player is invulnerable"), TEXT(__FUNCTION__));
			}
		}
		return;
	}
	
	if (bShowStatsLogs)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Invulnerability STARTED (%s)"), TEXT(__FUNCTION__), *GetOwner()->GetName());
	}
	
	GetWorld()->GetTimerManager().SetTimer(
		InvulnerabilityTimerHandle,
		this, &USSIMPlayerStatsComponent::EndInvulnerability,
		InvulnerabilityDuration,
		false);
	
	Health -= InDamageData.Value;
	Health = FMath::Clamp<int32>(Health, 0, MaxHealth);
	
	if (bShowStatsLogs)
	{
		UE_LOG(LogSSIMStatsCalculation, Log, TEXT("%s | Player Health: %d/%d"), TEXT(__FUNCTION__), 
														Health, 
														MaxHealth);
	}
	
	SSIMPlayer->GetContactDamageCollision()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SSIMPlayer->GetHitRegistrationCollision()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	bInvulnerable = true;
	OnInvulnerabilityStartedDelegate.Broadcast();
	
	Super::ReduceHealth(InDamageData);
}

void USSIMPlayerStatsComponent::IncreaseHealth(const int32 InHealValue)
{
	Health += InHealValue;
	Health = FMath::Clamp<int32>(Health, 0, MaxHealth);
	
	if (bShowStatsLogs)
	{
		UE_LOG(LogSSIMStatsCalculation, Log, TEXT("%s | Player Health: %d/%d"), TEXT(__FUNCTION__), 
														Health, 
														MaxHealth);
	}
	
	Super::IncreaseHealth(InHealValue);
}

void USSIMPlayerStatsComponent::StartHealing()
{
	if (!IsValid(HealingMontage))
	{
		if (bShowStatsLogs)
		{
			UE_LOG(LogSSIMValidations, Error, TEXT("%s | Hanging is not valid"), TEXT(__FUNCTION__));
		}
		return;
	}
	
	AnimInstance->Montage_Play(HealingMontage);
	StartAirHanging();
	
	bHealing = true;
	OnHealingStartedDelegate.Broadcast();
}

void USSIMPlayerStatsComponent::StartAirHanging()
{
	SSIMPlayer->GetCharacterMovement()->StopMovementImmediately();
	SSIMPlayer->GetCharacterMovement()->GravityScale = 0.0f;
	
	GetWorld()->GetTimerManager().SetTimer(
		AirHangingTimerHandle,
		this, &USSIMPlayerStatsComponent::EndAirHanging,
		AirHangingDuration,
		false
		);
}

void USSIMPlayerStatsComponent::EndAirHanging()
{
	IncreaseHealth(HealAmount);
	AnimInstance->Montage_Stop(HealingMontage->BlendOut.GetBlendTime());
	SSIMPlayer->SetPlayerGravityScaleToDefault();
	
	bHealing = false;
	OnHealingEndedDelegate.Broadcast();
}

void USSIMPlayerStatsComponent::EndInvulnerability()
{
	SSIMPlayer->GetContactDamageCollision()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SSIMPlayer->GetHitRegistrationCollision()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	bInvulnerable = false;
	OnInvulnerabilityEndedDelegate.Broadcast();
	
	if (bShowStatsLogs)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Invulnerability ENDED (%s)"), TEXT(__FUNCTION__), *GetOwner()->GetName());
	}
}


// DEBUG
void USSIMPlayerStatsComponent::DecrementHealth_DEBUG()
{
	Health--;
	Health = FMath::Clamp<int32>(Health, 0, MaxHealth);
	UE_LOG(LogSSIMStatsCalculation, Log, TEXT("%s | Player Health: %d/%d"), TEXT(__FUNCTION__), 
														Health, 
														MaxHealth);	
	
	FDamageData InDamageData
	{
		GetOwner(),
		1
	};
}

void USSIMPlayerStatsComponent::ReceiveHeal_DEBUG()
{
	StartHealing();
	
	OnHealReceivedDelegate.Broadcast(3);
}
 