// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerStatsComponent.h"

#include "SSIM/SSIM.h"
#include "SSIM/Core/Types/SSIMCombatDataTypes.h"


// My Functions
void USSIMPlayerStatsComponent::ReduceHealth(const FDamageData& InDamageData)
{
	Health -= InDamageData.DamageValue;
	Health = FMath::Clamp<int32>(Health, 0, MaxHealth);
	
	UE_LOG(LogSSIMStatsCalculation, Log, TEXT("%s | Player Health: %d/%d"), TEXT(__FUNCTION__), 
													Health, 
													MaxHealth);
	
	OnDamageReceivedDelegate.Broadcast(InDamageData);
}

void USSIMPlayerStatsComponent::IncreaseHealth(int32 InHealValue)
{
	Health += InHealValue;
	Health = FMath::Clamp<int32>(Health, 0, MaxHealth);
	
	UE_LOG(LogSSIMStatsCalculation, Log, TEXT("%s | Player Health: %d/%d"), TEXT(__FUNCTION__), 
													Health, 
													MaxHealth);
	
	OnHealReceivedDelegate.Broadcast(InHealValue);
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
	OnDamageReceivedDelegate.Broadcast(InDamageData);
}

void USSIMPlayerStatsComponent::IncrementHealth_DEBUG()
{
	Health++;
	Health = FMath::Clamp<int32>(Health, 0, MaxHealth);
	UE_LOG(LogSSIMStatsCalculation, Log, TEXT("%s | Player Health: %d/%d"), TEXT(__FUNCTION__), 
														Health, 
														MaxHealth);	
	
	OnHealReceivedDelegate.Broadcast(1);
}
 