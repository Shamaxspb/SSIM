// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMBaseDamageReactionComponent.h"
#include "SSIM/Core/Types/SSIMCombatDataTypes.h"

#include "SSIMPlayerDamageReactionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaggerStartedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaggerEndedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInvulnerabilityStartedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInvulnerabilityEndedSignature);


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
	FOnInvulnerabilityStartedSignature OnInvulnerabilityStartedDelegate;
	FOnInvulnerabilityEndedSignature OnInvulnerabilityEndedDelegate;
	
	
protected:
	
	UPROPERTY()
	TObjectPtr<ASSIMPlayer> SSIMPlayer;
	
	// Damage processing
#pragma region Stagger
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DamageProcessing|Animations")
	TObjectPtr<UAnimMontage> StaggeredFirstFrame;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DamageProcessing")
	float StopFrameDuration = 0.2f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DamageProcessing")
	float StaggerDuration = 0.4f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DamageProcessing")
	float InvulnerabilityDuration = 1.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DamageProcessing")
	float ReboundVelocityY = 1000.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DamageProcessing")
	float ReboundVelocityZ = 2000.f;

private:
	float StaggeredFirstFrameBlendInTime = 0.1f;
	
	TArray<FStaggerSequenceStep> Steps;
	int32 CurrentStaggerSequenceStep = 0;
	FTimerHandle StaggerSequenceHandle;
	
#pragma endregion Stagger
	
#pragma region Metadata

	FVector ReboundLaunchVelocity = FVector::ZeroVector;
	
#pragma endregion Metadata
	
// Overriden Functions
protected:
	virtual void BeginPlay() override;

// My Functions
protected:
	virtual void SetReferences() override;
	
	virtual void OnDamageReceivedHandler(const FDamageData& InDamageData) override;

private:
	// Damage processing
	UFUNCTION()
	void InitStagger();
	void StartStaggerSequence();
	void ExecuteNextStaggerSequenceStep();
	void StartStopFrame() const;
	void EndStopFrame() const;
	void StartStagger();
	void EndStagger() const;
	void EndInvulnerability() const;
	
	// Debug
protected:
	virtual void ReboundDrawDebug() override;
	
};
