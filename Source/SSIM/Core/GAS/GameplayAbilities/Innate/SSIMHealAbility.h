// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIM/Core/GAS/GameplayAbilities/SSIMGameplayAbility.h"

#include "SSIMHealAbility.generated.h"


UCLASS(Abstract, Blueprintable)
class SSIM_API USSIMHealAbility : public USSIMGameplayAbility
{
	GENERATED_BODY()
	
// Variables
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animation")
	TObjectPtr<UAnimMontage> HealingMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Config")	
	int32 HealAmount = 3;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Config")	
	float HealingAirTime = 1.f;
	
// Overriden Functions
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
								 const FGameplayAbilityActorInfo* ActorInfo, 
								 const FGameplayAbilityActivationInfo ActivationInfo, 
								 const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, 
							const FGameplayAbilityActorInfo* ActorInfo, 
							const FGameplayAbilityActivationInfo ActivationInfo, 
							bool bReplicateEndAbility, 
							bool bWasCancelled) override;
	
// Custom Functions
private:
	UFUNCTION()
	void EndAirHanging();	
	
};
