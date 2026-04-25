// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMBaseDamageReactionComponent.h"

#include "SSIMEnemyDamageReactionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartStaggerSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndStaggerSignature);

class ASSIMBaseEnemy;
class USSIMEnemyStatsComponent;


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SSIM_API USSIMEnemyDamageReactionComponent : public USSIMBaseDamageReactionComponent
{
	GENERATED_BODY()
	
// Variables
	// Delegates
public:
	FOnStartStaggerSignature OnStartStaggerDelegate;
	FOnEndStaggerSignature OnEndStaggerDelegate;
	
	
protected:
	UPROPERTY()
	TObjectPtr<ASSIMBaseEnemy> SSIMEnemy;
	UPROPERTY()
	TObjectPtr<USSIMEnemyStatsComponent> EnemyStatsComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DamageProcessing|Animations", meta = (DisplayPriority = 2))
	TObjectPtr<UAnimMontage> BackStaggeredMontage;
	
#pragma region Stagger
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Stagger")
	float StaggerDuration = 0.5f;
	
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Stagger|Rebound")
	float ReboundVelocityZ = 250.f;
	
#pragma endregion Rebound

#pragma region Metadata
	
	FVector RotatedDirection = FVector::ZeroVector;
	
	EPlayerAttackDirectionType PlayerAttackDirectionType;
	
#pragma endregion Metadata

// Overriden Functions
protected:
	virtual void BeginPlay() override;
	virtual void SetReferences() override;
	
// My Functions
public:
	void ReceivePlayerAttackDirectionType(EPlayerAttackDirectionType InPlayerAttackDirectionType);
	
protected:
	virtual void OnDamageReceivedHandler(const FDamageData& InDamageData) override;
	
private:
	void StartStagger();
	void EndStagger() const;
	
	void ReboundOnHit();
	
	UAnimMontage* SelectStaggerMontage() const;
	
	// Debug
protected:
	virtual void ReboundDrawDebug() override;
};
