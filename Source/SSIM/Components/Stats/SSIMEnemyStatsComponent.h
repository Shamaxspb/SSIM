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
	UPROPERTY(BlueprintReadOnly, Category = "SSIM|Combat")
	EEnemyState EnemyState;
	
	
// My Functions
public:
	UFUNCTION(BlueprintCallable, Category = "SSIM|Combat|Stats")
	virtual void ReduceHealth(const FDamageData& InDamageData) override;
	
};
