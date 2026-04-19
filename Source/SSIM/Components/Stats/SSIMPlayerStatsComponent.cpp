// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerStatsComponent.h"

#include "SSIM/SSIM.h"


// My Functions
void USSIMPlayerStatsComponent::SetReceivedDamage(int32 InReceivedDamage)
{
	ReceivedDamage = InReceivedDamage;
}

void USSIMPlayerStatsComponent::ReduceHealth()
{
	Health -= ReceivedDamage;
	Health = FMath::Clamp<int32>(Health, 0, MaxHealth);
	
	UE_LOG(LogSSIMStatsCalculation, Log, TEXT("%s | Player Health: %d/%d"), TEXT(__FUNCTION__), 
													Health, 
													MaxHealth);
	
	OnDamageReceivedDelegate.Broadcast(Health, DamageInstigator);
}


// DEBUG
void USSIMPlayerStatsComponent::IncrementHealth_DEBUG()
{
	Health++;
	UE_LOG(LogSSIMStatsCalculation, Log, TEXT("%s | Player Health: %d/%d"), TEXT(__FUNCTION__), 
														Health, 
														MaxHealth);	
	OnHealReceivedDelegate.Broadcast(Health);
}
 