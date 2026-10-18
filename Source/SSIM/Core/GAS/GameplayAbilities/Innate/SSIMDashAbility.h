// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIM/Core/GAS/GameplayAbilities/SSIMGameplayAbility.h"
#include "SSIMDashAbility.generated.h"


class UAbilityTask_WaitGameplayEvent;
class UAbilityTask_WaitDelay;

UCLASS(Abstract, Blueprintable)
class SSIM_API USSIMDashAbility : public USSIMGameplayAbility
{
	GENERATED_BODY()
	
// Variables
#pragma region Settings
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animation")
	TObjectPtr<UAnimMontage> DashMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Config")
	float DashDuration = 0.25f;
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Config")
	float DashCooldown = 2.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Config")
	float DashVelocity = 2000.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Config")
	float DashBrakingDecelerationWalking = 1000.f;
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Config")
	float DashBrakingDecelerationFalling = 1000.f;
	
	

	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DamageCollision")
	float DashHitRegistrationCollisionHalfHeight = 80.f;
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DamageCollision")
	float DashHitRegistrationCollisionRadius = 24.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DamageCollision")
	float DashContactDamageCollisionHalfHeight = 105.f;
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DamageCollision")
	float DashContactDamageCollisionRadius = 35.f;
#pragma endregion Settings
	
#pragma region Metadata
private:
	UPROPERTY()
	UAbilityTask_WaitDelay* WaitDashEnd;
	
	bool bStartedInAir = false;
	
	FTimerHandle DashInProcessTimerHandle;
#pragma endregion Metadata
	
private:
	FName SwitchMovementModeNotifyName = TEXT("SwitchMovementMode");
	TWeakObjectPtr<UAnimInstance> BoundAnimInstance;
	int32 ActiveMontageInstanceID = INDEX_NONE;
	
	UPROPERTY(Transient)
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitTakeDamageTask;
	UPROPERTY(Transient)
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitPogoStartedTask;
	
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
private:
	void SetCollisionParametersToDash() const;
	void SetCollisionParametersToDefault() const;
	
	UFUNCTION()
	void HandleSwitchMovementModeMontageNotify(FName NotifyName, const FBranchingPointNotifyPayload& Payload);
	
	UFUNCTION()
	void EndDashByTakingDamage(FGameplayEventData Payload);
	UFUNCTION()
	void EndDashByPogo(FGameplayEventData Payload);
	UFUNCTION()
	void EndDashByLanding(const FHitResult& Hit);
	UFUNCTION()
	void EndDashByDuration();
};


