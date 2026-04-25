// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMBaseStatsComponent.h"
#include "SSIM/Core/Types/SSIMCombatDataTypes.h"

#include "SSIMEnemyStatsComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SSIM_API USSIMEnemyStatsComponent : public USSIMBaseStatsComponent
{
	GENERATED_BODY()

// Variables
public:
	UPROPERTY()
	EEnemyState EnemyState;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SSIM|Combat|Stats|Health", meta = (ClampMin = 0))
	int32 MaxHealth = 50;
	
	UPROPERTY(BlueprintReadWrite, Category = "SSIM|Combat|Stats|Health", meta = (ClampMin = 0))
	int32 Health = MaxHealth;
	
// My Functions
public:
	UFUNCTION(BlueprintCallable, Category = "SSIM|Combat|Stats")
	virtual void ReduceHealth(const FDamageData& InDamageData) override;
	
};
