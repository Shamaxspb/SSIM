// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIM/Core/GAS/GameplayAbilities/SSIMGameplayAbility.h"

#include "SSIMPlayerAttackAbility.generated.h"


class UAbilityTask_WaitGameplayEvent;

UCLASS(Abstract)
class SSIM_API USSIMPlayerAttackAbility : public USSIMGameplayAbility
{
	GENERATED_BODY()
	
// Variables
protected:
	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations|Attack")
	TObjectPtr<UAnimMontage> AttackMontage;*/
	
private:
	UPROPERTY(Transient)
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitTakeDamageTask;
	
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
protected:
	UFUNCTION()
	void HandleTakeDamage(FGameplayEventData Payload);
	
	virtual UAnimMontage* GetAttackMontage();
	
private:
	void HandleEndMontage(UAnimMontage* PogoMontage, bool Interrupted);
	
};
