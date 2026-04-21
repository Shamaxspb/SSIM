// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMBaseStatsComponent.h"

#include "SSIMPlayerStatsComponent.generated.h"

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
	
	UPROPERTY(BlueprintReadWrite, Category = "SSIM|Combat|Stagger", meta = (ClampMin = 0))	
	bool bStaggered;
	
	UPROPERTY(BlueprintReadWrite, Category = "SSIM|Combat|Stagger", meta = (ClampMin = 0))	
	bool bInvulnerable;
	
	
// My Functions
public:
	UFUNCTION(BlueprintCallable, Category = "SSIM|Combat|Stats")
	virtual void ReduceHealth(const FDamageData& InDamageData) override;

	
// DEBUG
public:
	UFUNCTION(BlueprintCallable)
	void IncrementHealth_DEBUG();
};
