// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMEnemyHealthComponent.h"

#include "SSIM/SSIM.h"


// My Functions
void USSIMEnemyHealthComponent::ReduceHealth(float InHealth)
{
	Health -= InHealth;
	Health = FMath::Clamp(Health, 0.f, MaxHealth);
	
	UE_LOG(LogSSIMStatsCalculation, Log, TEXT("%s | %s Health: %f/%f"),TEXT(__FUNCTION__), 
										 *GetOwner()->GetName(),
										 Health,
										 MaxHealth);
	
	if (Health <= 0.0f)
	{
		// Call function DeathProcessing()
		UE_LOG(LogSSIMStatsCalculation, Log, TEXT("%s | DeathProcessing() called"),TEXT(__FUNCTION__));
	}
}
