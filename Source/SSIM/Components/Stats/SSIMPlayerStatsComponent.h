// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMBaseStatsComponent.h"

#include "SSIMPlayerStatsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInvulnerabilityStartedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInvulnerabilityEndedSignature);

class ASSIMPlayer;
struct FStaggerSequenceStep;
struct FDamageData;


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SSIM_API USSIMPlayerStatsComponent : public USSIMBaseStatsComponent
{
	GENERATED_BODY()

// Variables
public:
	//Delegates
	FOnInvulnerabilityStartedSignature OnInvulnerabilityStartedDelegate;
	FOnInvulnerabilityEndedSignature OnInvulnerabilityEndedDelegate;
	
	bool bInvulnerable = false;
	bool bDead = false;

	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "SSIM")	
	int32 HealAmount = 3;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DamageProcessing")	
	float InvulnerabilityDuration = 1.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Animation")
	TObjectPtr<UAnimMontage> HealingMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM")	
	float AirHangingDuration = 1.f;
	
private:
	UPROPERTY()
	TObjectPtr<ASSIMPlayer> SSIMPlayer;
	
	FTimerHandle InvulnerabilityTimerHandle;
	FTimerHandle AirHangingTimerHandle;

	// Debug
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SSIM|DEBUG", 
		meta = (EditCondition = "bShowStatsLogs", EditConditionHides, DisplayAfter = "bShowStatsLogs"))
	bool bShowInvulnerabilityCheck;
	
	
// My Functions
public:
	UFUNCTION(BlueprintCallable, Category = "SSIM|Stats")
	virtual void ReduceHealth(const FDamageData& InDamageData) override;
	
	UFUNCTION(BlueprintCallable, Category = "SSIM|Stats")
	virtual void IncreaseHealth(const int32 InHealValue) override;
	
	void StartInvulnerability();
	void EndInvulnerability();
	
protected:
	virtual void SetReferences() override;
	virtual void DeathProcessing(const FDamageData& InDamageData) override;
	
	
// DEBUG
public:
	UFUNCTION(BlueprintCallable, Category = "SSIM|DEBUG")
	void DecrementHealth_DEBUG();
	
};
