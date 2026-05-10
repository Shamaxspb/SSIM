// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMBaseStatsComponent.h"


// My Functions
void USSIMBaseStatsComponent::ReduceHealth(const FDamageData& InDamageData)
{
	OnDamageReceivedDelegate.Broadcast(InDamageData);
}

void USSIMBaseStatsComponent::IncreaseHealth(int32 InHealValue)
{
	OnHealReceivedDelegate.Broadcast(InHealValue);
}

void USSIMBaseStatsComponent::DeathProcessing(const FDamageData& InDamageData)
{
	OnCharacterDiedDelegate.Broadcast();
}
