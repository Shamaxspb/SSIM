// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIM/Core/GAS/GameplayAbilities/SSIMGameplayAbility.h"
#include "SSIMDashAbility.generated.h"


UCLASS()
class SSIM_API USSIMDashAbility : public USSIMGameplayAbility
{
	GENERATED_BODY()
	
	// Overriden Functions
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
								 const FGameplayAbilityActorInfo* ActorInfo, 
								 const FGameplayAbilityActivationInfo ActivationInfo, 
								 const FGameplayEventData* TriggerEventData) override;
};
