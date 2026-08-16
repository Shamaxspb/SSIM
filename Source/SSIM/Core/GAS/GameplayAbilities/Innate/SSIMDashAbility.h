// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIM/Core/GAS/GameplayAbilities/SSIMGameplayAbility.h"
#include "SSIMDashAbility.generated.h"


UCLASS()
class SSIM_API USSIMDashAbility : public USSIMGameplayAbility
{
	GENERATED_BODY()
	
// Variables
#pragma region Settings
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animation")
	TObjectPtr<UAnimMontage> PlayerDashMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Config")
	float DashDuration = 0.25f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Config")
	float DashVelocity = 2000.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Config")
	float DashGravityScale = 0.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Config")
	float DashBrakingDecelerationWalking = 1000.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Config")
	float DashBrakingDecelerationFalling = 1000.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Config")
	float DashCooldown = 2.f;

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
	FTimerHandle DashInProcessTimerHandle;
#pragma endregion Metadata
	
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
	UFUNCTION()
	void HandleTakeDamage(FGameplayEventData Payload);
	
	void StartDash();

};


