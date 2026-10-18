// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SSIMGameplayAbility.generated.h"

class ASSIMPlayer;

UCLASS(Abstract)
class SSIM_API USSIMGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
// Variables
protected:
	UPROPERTY(BlueprintReadOnly, Category = "SSIM|References")
	TObjectPtr<ASSIMPlayer> Player;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Debug")
	bool bShowDebug;
	
// Overriden Functions
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
								 const FGameplayAbilityActorInfo* ActorInfo, 
								 const FGameplayAbilityActivationInfo ActivationInfo, 
								 const FGameplayEventData* TriggerEventData) override;
	
// Custom Functions
	
	
};
