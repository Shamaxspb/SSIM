// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMPlayerAttackAbility.h"

#include "SSIMUpwardAttack.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SSIM_API USSIMUpwardAttack : public USSIMPlayerAttackAbility
{
	GENERATED_BODY()
	
// Variables
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations")
	TArray<TObjectPtr<UAnimMontage>> UpwardAttackMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations")
	TArray<TObjectPtr<UAnimMontage>> AirUpwardAttackMontages;

	
// Overriden Functions
protected:
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
	virtual UAnimMontage* GetAttackMontage() override;
};
