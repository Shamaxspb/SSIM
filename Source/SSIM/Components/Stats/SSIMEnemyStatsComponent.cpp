// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMEnemyStatsComponent.h"

#include "SSIM/SSIM.h"


// My Functions
void USSIMEnemyStatsComponent::ReduceHealth(const FDamageData& InDamageData)
{
	Health -= InDamageData.Value;
	Health = FMath::Clamp<int32>(Health, 0, MaxHealth);
	
	UE_LOG(LogSSIMStatsCalculation, Log, TEXT("%s | %s Health: %d/%d"),TEXT(__FUNCTION__), 
										 *GetOwner()->GetName(),
										 Health,
										 MaxHealth);
	
	OnDamageReceivedDelegate.Broadcast(InDamageData);
}
