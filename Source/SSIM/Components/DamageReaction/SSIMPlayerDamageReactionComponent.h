// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMBaseDamageReactionComponent.h"
#include "SSIM/Core/Types/SSIMCombatDataTypes.h"

#include "SSIMPlayerDamageReactionComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndInvulnerabilitySignature);

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
	FOnEndInvulnerabilitySignature OnEndInvulnerabilityDelegate;
	
protected:
	// Damage processing
	FDamageData DamageData;
	
#pragma region Stagger
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DamageProcessing|Animations")
	TObjectPtr<UAnimMontage> StaggeredFirstFrame;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DamageProcessing|Animations")
	TObjectPtr<UAnimMontage> StaggeredAnimation;
	
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
	
	// DEBUG
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DEBUG")
	bool bReboundShowDebug;
	
	UPROPERTY(EditAnywhere, Category = "SSIM|DEBUG", meta = (EditCondition = "bReboundShowDebug", EditConditionHides))
	bool bDrawReboundDirectionArrow;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DEBUG", meta = (EditCondition = "bReboundShowDebug && bDrawReboundDirectionArrow", EditConditionHides))
	float DrawDuration = 3.f;
	
	UPROPERTY(EditAnywhere, Category = "SSIM|DEBUG", meta = (EditCondition = "bReboundShowDebug && bDrawReboundDirectionArrow", EditConditionHides))
	FLinearColor ReboundDirectionArrowColor = FLinearColor(1.f, 0.148f, 0.106f, 1.f);
	
private:
	float StaggeredFirstFrameBlendInTime = 0.1f;
	
	TArray<FStaggerSequenceStep> Steps;
	int32 CurrentStaggerSequenceStep = 0;
	FTimerHandle StaggerSequenceHandle;
	
#pragma endregion Stagger
	
	
// Overriden Functions
protected:
	virtual void BeginPlay() override;

// My Functions
protected:
	virtual void OnDamageReceivedHandler(FDamageData InDamageData) override;
	
private:
	// Damage processing
	UFUNCTION()
	void InitStagger(const FDamageData DamageData);
	void StartStaggerSequence();
	void ExecuteNextStaggerSequenceStep();
	void StartStopFrame() const;
	void EndStopFrame() const;
	void StartStagger() const;
	void EndStagger() const;
	void EndInvulnerability() const;
	
	// DEBUG
protected:
	UFUNCTION(BlueprintCallable, Category = "SSIM|DEBUG")
	void ManualStagger_DEBUG();
};
