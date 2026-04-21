// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIM/Components/SSIMBaseComponent.h"
#include "SSIM/Core/Types/SSIMCombatDataTypes.h"

#include "SSIMBaseStatsComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageReceivedSignature, FDamageData, DamageData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealReceivedSignature, int32, HealValue);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SSIM_API USSIMBaseStatsComponent : public USSIMBaseComponent
{
	GENERATED_BODY()
	
// Variables
public:
	UPROPERTY(BlueprintReadWrite, Category = "SSIM|UI")
	FOnDamageReceivedSignature OnDamageReceivedDelegate;
	
	UPROPERTY(BlueprintReadWrite, Category = "SSIM|UI")
	FOnHealReceivedSignature OnHealReceivedDelegate;
	
// My Functions
public:
	virtual void ReduceHealth(const FDamageData& InDamageData);
	
	virtual void IncreaseHealth(int32 InHealValue);
};
