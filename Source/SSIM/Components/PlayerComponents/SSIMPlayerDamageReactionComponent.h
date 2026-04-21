// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SSIM/Components/SSIMBaseComponent.h"
#include "SSIMPlayerDamageReactionComponent.generated.h"


class USSIMPlayerStatsComponent;
class ASSIMPlayer;

struct FStaggerSequenceStep
{
	float Delay;
	TFunction<void()> Function;
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SSIM_API USSIMPlayerDamageReactionComponent : public USSIMBaseComponent
{
	GENERATED_BODY()

// Variables
protected:
	// Damage processing
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Animations|Stagger")
	TObjectPtr<UAnimMontage> StaggeredFirstFrame;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Animations|Stagger")
	TObjectPtr<UAnimMontage> StaggeredAnimation;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Stagger")
	float StopFrameDuration = 0.2f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Stagger")
	float StaggerDuration = 0.4f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Stagger")
	float InvulnerabilityDuration = 1.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Stagger")
	float ReboundVelocityY = 1000.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Stagger")
	float ReboundVelocityZ = 2000.f;
	
private:
	float StaggeredFirstFrameBlendInTime = 0.1f;
	
	TArray<FStaggerSequenceStep> Steps;
	int32 CurrentStaggerSequenceStep = 0;
	FTimerHandle StaggerSequenceHandle;
	
	UPROPERTY()
	TObjectPtr<USSIMPlayerStatsComponent> PlayerStatsComponent;
	
// Overriden Functions
public:
	USSIMPlayerDamageReactionComponent();

protected:
	virtual void BeginPlay() override;

// My Functions
protected:
	virtual void SetReferences() override;
	
private:
	// Damage processing
	void InitStagger();
	void StartStaggerSequence();
	void StartStopFrame() const;
	void StopFrameToStagger();
	void EndStagger();
	void EndInvulnerability();
	
};
