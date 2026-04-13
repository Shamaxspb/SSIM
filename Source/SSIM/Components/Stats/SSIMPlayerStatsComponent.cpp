// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerStatsComponent.h"

#include "SSIM/Characters/Player/SSIMPlayer.h"

void USSIMPlayerStatsComponent::SetReceivedDamage(int32 InReceivedDamage)
{
	ReceivedDamage = InReceivedDamage;
}

void USSIMPlayerStatsComponent::ReduceHealth()
{
	
}
