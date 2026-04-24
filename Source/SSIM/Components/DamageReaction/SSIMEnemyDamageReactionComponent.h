// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMBaseDamageReactionComponent.h"

#include "SSIMEnemyDamageReactionComponent.generated.h"

class ASSIMBaseEnemy;
DECLARE_MULTICAST_DELEGATE(FEndStaggerSignature);

class USSIMEnemyStatsComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SSIM_API USSIMEnemyDamageReactionComponent : public USSIMBaseDamageReactionComponent
{
	GENERATED_BODY()
	
// Variables
	// Delegates
	FEndStaggerSignature EndStaggerDelegate;
	
protected:
	UPROPERTY()
	TObjectPtr<ASSIMBaseEnemy> SSIMEnemy;
	UPROPERTY()
	TObjectPtr<USSIMEnemyStatsComponent> EnemyStatsComponent;
	
#pragma region Stagger
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Stagger")
	float StaggerDuration;
	
private:
	FTimerHandle StaggerTimerHandle;
	FTimerDelegate StaggerTimerDelegate;
	
#pragma endregion Stagger	

#pragma region Rebound

protected:
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Stagger|Rebound")
	float ReboundAngle = 45.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Stagger|Rebound")
	float ReboundVelocityCoef = 700.f;

#pragma endregion Rebound

#pragma region Metadata
	
	FVector RotatedDirection = FVector::ZeroVector;
	
#pragma endregion Metadata

// Overriden Functions
protected:
	virtual void BeginPlay() override;
	virtual void SetReferences() override;
	
// My Functions
protected:
	virtual void OnDamageReceivedHandler(const FDamageData& InDamageData) override;
	
private:
	void StartStagger();
	void EndStagger();
	
	void ReboundOnHit();
	
	// Debug
protected:
	virtual void ReboundDrawDebug() override;
};
