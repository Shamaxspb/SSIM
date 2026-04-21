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


// DEBUG
void USSIMPlayerStatsComponent::IncrementHealth_DEBUG()
{
	Health++;
	UE_LOG(LogSSIMStatsCalculation, Log, TEXT("%s | Player Health: %d/%d"), TEXT(__FUNCTION__), 
														Health, 
														MaxHealth);	
	FDamageData DebugDamageData 
	{
		DebugDamageData.DamageInstigator = nullptr,
		DebugDamageData.DamageValue = 1
	};
	OnHealReceivedDelegate.Broadcast(DebugDamageData);
}
 