// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMBaseDamageReactionComponent.h"
#include "SSIM/Core/Types/SSIMCombatDataTypes.h"

#include "SSIMPlayerDamageReactionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaggerStartedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaggerEndedSignature);


class USSIMPlayerStatsComponent;
class ASSIMPlayer;


struct FStaggerSequenceStep
{
	float Delay;
	TFunction<void()> Function;
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SSIM_API USSIMPlayerDamageReactionComponent : public USSIMBaseDamageReactionComponent
{
	GENERATED_BODY()

// Variables
public:
	// Delegates
	UPROPERTY(BlueprintReadWrite, Category = "SSIM|DamageProcessing")
	FOnStaggerStartedSignature OnStaggerStartedDelegate;
	FOnStaggerEndedSignature OnStaggerEndedDelegate;
	
	bool bStaggered;
	
protected:
	
	UPROPERTY()
	TObjectPtr<ASSIMPlayer> SSIMPlayer;
	
	// Damage processing
#pragma region Stagger
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Animations")
	TObjectPtr<UAnimMontage> StaggeredFirstFrame;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DamageProcessing")
	float StopFrameDuration = 0.2f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DamageProcessing")
	float StaggerDuration = 0.4f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DamageProcessing")
	float ReboundVelocityY = 1000.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DamageProcessing")
	float ReboundVelocityZ = 2000.f;
	
	
	

private:
	float StaggeredFirstFrameBlendInTime = 0.1f;
	TArray<FStaggerSequenceStep> Steps;
	FTimerHandle StaggerSequenceHandle;
	
#pragma endregion Stagger
	
#pragma region Death

public:
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DeathProcessing")
	float DeathStateDuration = 1.5f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DeathProcessing")
	float DeathReboundVelocityY = 1000.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DeathProcessing")
	float DeathReboundVelocityZ = 2000.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DeathProcessing")
	float DeathReboundInitialGravity = 2.f;
	
private:
	FVector InitialVelocity = FVector::ZeroVector;

#pragma endregion Death
	
#pragma region Metadata

	int32 CurrentStaggerSequenceStep = 0;
	
	bool bShouldInterpolateGravity = false;
	float GravityInterpolationElapsedTime = 0.f;
	
#pragma endregion Metadata
	
// Overriden Functions
public:
	USSIMPlayerDamageReactionComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

// My Functions
protected:
	virtual void SetReferences() override;
	
	virtual void OnDamageReceivedHandler(const FDamageData& InDamageData) override;

	virtual void ReboundOnHit(UAnimMontage* InReboundMontage) override;
	
private:
	// Damage processing
	UFUNCTION()
	void InitStagger();
	void StartStaggerSequence();
	void ExecuteNextStaggerSequenceStep();
	void StartStopFrame() const;
	void EndStopFrame() const;
	void StartStagger();
	void EndStagger();
	
	void LethalDamageReaction();
	void InterpolateGravityToZeroOnDeath(float DeltaTime);
	void EndReboundOnDeath();
	
};
