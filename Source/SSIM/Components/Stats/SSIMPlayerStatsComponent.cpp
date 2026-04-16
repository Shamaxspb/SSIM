// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerStatsComponent.h"

void USSIMPlayerStatsComponent::SetReceivedDamage(int32 InReceivedDamage)
{
	ReceivedDamage = InReceivedDamage;
}

void USSIMPlayerStatsComponent::ReduceHealth()
{
	Health--;
	OnDamageReceived.Broadcast();
}
