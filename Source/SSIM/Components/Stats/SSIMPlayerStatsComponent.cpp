// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerStatsComponent.h"

#include "SSIM/SSIM.h"
#include "SSIM/Core/Types/SSIMCombatDataTypes.h"


// My Functions
void USSIMPlayerStatsComponent::ReduceHealth(const FDamageData& InDamageData)
{
	if (bInvulnerable)
	{
		if (bShowStatsLogs)
		{
			UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Player is invulnerable"), TEXT(__FUNCTION__));
		}
		return;
	}
	
	bInvulnerable = true;
	OnInvulnerabilityStartedDelegate.Broadcast();
	
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
	
	Super::ReduceHealth(InDamageData);
}

void USSIMPlayerStatsComponent::IncreaseHealth(int32 InHealValue)
{
	Health += InHealValue;
	Health = FMath::Clamp<int32>(Health, 0, MaxHealth);
	
	if (bShowStatsLogs)
	{
		UE_LOG(LogSSIMStatsCalculation, Log, TEXT("%s | Player Health: %d/%d"), TEXT(__FUNCTION__), 
														Health, 
														MaxHealth);
	}
}

void USSIMPlayerStatsComponent::EndInvulnerability()
{
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

void USSIMPlayerStatsComponent::IncrementHealth_DEBUG()
{
	Health++;
	Health = FMath::Clamp<int32>(Health, 0, MaxHealth);
	UE_LOG(LogSSIMStatsCalculation, Log, TEXT("%s | Player Health: %d/%d"), TEXT(__FUNCTION__), 
														Health, 
														MaxHealth);	
	
}
 