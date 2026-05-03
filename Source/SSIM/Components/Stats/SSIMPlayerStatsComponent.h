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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SSIM|Stats", meta = (ClampMin = 0))	
	int32 MaxHealth = 5;
	
	UPROPERTY(BlueprintReadWrite, Category = "SSIM|Stats", meta = (ClampMin = 0))	
	int32 Health = MaxHealth;
	
	bool bInvulnerable;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DamageProcessing")	
	float InvulnerabilityDuration = 1.f;
	
private:
	UPROPERTY()
	TObjectPtr<ASSIMPlayer> SSIMPlayer;
	
	FTimerHandle InvulnerabilityTimerHandle;

	// Debug
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SSIM|DEBUG", 
		meta = (EditCondition = "bShowStatsLogs", EditConditionHides, DisplayAfter = "bShowStatsLogs"))
	bool bShowInvulnerabilityCheck;
	
	
// My Functions
	virtual void BeginPlay() override;
	
	
// My Functions
public:
	UFUNCTION(BlueprintCallable, Category = "SSIM|Combat|Stats")
	virtual void ReduceHealth(const FDamageData& InDamageData) override;
	
	UFUNCTION(BlueprintCallable, Category = "SSIM|Combat|Stats")
	virtual void IncreaseHealth(int32 InHealValue) override;
	
	void EndInvulnerability();
	
protected:
	virtual void SetReferences() override;
	
// DEBUG
public:
	UFUNCTION(BlueprintCallable, Category = "SSIM|DEBUG")
	void DecrementHealth_DEBUG();
	
	UFUNCTION(BlueprintCallable, Category = "SSIM|DEBUG")
	void IncrementHealth_DEBUG();
};
