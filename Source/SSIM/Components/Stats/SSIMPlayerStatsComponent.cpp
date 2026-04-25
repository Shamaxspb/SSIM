// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerStatsComponent.h"

#include "SSIM/SSIM.h"
#include "SSIM/Core/Types/SSIMCombatDataTypes.h"


void USSIMPlayerStatsComponent::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// My Functions
void USSIMPlayerStatsComponent::ReduceHealth(const FDamageData& InDamageData)
{
	
	
	if (bInvulnerable)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Player is invulnerable"), TEXT(__FUNCTION__));
		return;
	}
	
	Health -= InDamageData.Value;
	Health = FMath::Clamp<int32>(Health, 0, MaxHealth);
	
	UE_LOG(LogSSIMStatsCalculation, Log, TEXT("%s | Player Health: %d/%d"), TEXT(__FUNCTION__), 
													Health, 
													MaxHealth);
	
	Super::ReduceHealth(InDamageData);
}

void USSIMPlayerStatsComponent::IncreaseHealth(int32 InHealValue)
{
	Health += InHealValue;
	Health = FMath::Clamp<int32>(Health, 0, MaxHealth);
	
	UE_LOG(LogSSIMStatsCalculation, Log, TEXT("%s | Player Health: %d/%d"), TEXT(__FUNCTION__), 
													Health, 
													MaxHealth);
	
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
 