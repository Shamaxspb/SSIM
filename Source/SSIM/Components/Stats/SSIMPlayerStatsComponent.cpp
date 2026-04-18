// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerStatsComponent.h"

#include "SSIM/SSIM.h"

void USSIMPlayerStatsComponent::SetReceivedDamage(int32 InReceivedDamage)
{
	ReceivedDamage = InReceivedDamage;
}

void USSIMPlayerStatsComponent::ReduceHealth()
{
	Health -= ReceivedDamage;
	
	UE_LOG(LogSSIMStatsCalculation, Log, TEXT("%s | Player Health: %d/%d"), TEXT(__FUNCTION__), Health, MaxHealth);
	
	OnDamageReceivedDelegate.Broadcast(Health);
}


// DEBUG
void USSIMPlayerStatsComponent::IncrementHealth_DEBUG()
{
	Health++;
	OnHealReceivedDelegate.Broadcast(Health);
}
 