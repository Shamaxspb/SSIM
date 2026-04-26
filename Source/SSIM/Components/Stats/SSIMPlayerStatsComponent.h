// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMBaseStatsComponent.h"

#include "SSIMPlayerStatsComponent.generated.h"

class USSIMPlayerDamageReactionComponent;
class ASSIMPlayer;
struct FStaggerSequenceStep;
struct FDamageData;


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SSIM_API USSIMPlayerStatsComponent : public USSIMBaseStatsComponent
{
	GENERATED_BODY()

// Variables
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SSIM|Combat|Stats|Health", meta = (ClampMin = 0))	
	int32 MaxHealth = 5;
	
	UPROPERTY(BlueprintReadWrite, Category = "SSIM|Combat|Stats|Health", meta = (ClampMin = 0))	
	int32 Health = MaxHealth;
	
	UPROPERTY(BlueprintReadWrite, Category = "SSIM|Combat|Stagger")	
	bool bInvulnerable;
	
private:
	UPROPERTY()
	TObjectPtr<USSIMPlayerDamageReactionComponent> PlayerDamageReactionComponent;
	
// Overriden Functions
protected:
	virtual void BeginPlay() override;
	virtual void SetReferences() override;
	
	
// My Functions
public:
	UFUNCTION(BlueprintCallable, Category = "SSIM|Combat|Stats")
	virtual void ReduceHealth(const FDamageData& InDamageData) override;
	
	UFUNCTION(BlueprintCallable, Category = "SSIM|Combat|Stats")
	virtual void IncreaseHealth(int32 InHealValue) override;

private:
	UFUNCTION()
	void OnInvulnerabilityStartedHandler();
	UFUNCTION()
	void OnInvulnerabilityEndedHandler();
	
	
// DEBUG
public:
	UFUNCTION(BlueprintCallable, Category = "SSIM|DEBUG")
	void DecrementHealth_DEBUG();
	
	UFUNCTION(BlueprintCallable, Category = "SSIM|DEBUG")
	void IncrementHealth_DEBUG();
};
