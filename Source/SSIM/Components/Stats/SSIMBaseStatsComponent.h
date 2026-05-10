// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIM/Components/SSIMBaseComponent.h"
#include "SSIM/Core/Types/SSIMCombatDataTypes.h"

#include "SSIMBaseStatsComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageReceivedSignature, const FDamageData&, InDamageData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealReceivedSignature, int32, HealValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterDiedSignature);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SSIM_API USSIMBaseStatsComponent : public USSIMBaseComponent
{
	GENERATED_BODY()
	
// Variables
public:
	// Delegates
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "SSIM|Health")
	FOnDamageReceivedSignature OnDamageReceivedDelegate;
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "SSIM|Health")
	FOnHealReceivedSignature OnHealReceivedDelegate;
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "SSIM|Health")
	FOnCharacterDiedSignature OnCharacterDiedDelegate;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SSIM|DEBUG")
	bool bShowStatsLogs;
	
	
// My Functions
public:
	virtual void ReduceHealth(const FDamageData& InDamageData);
	
	virtual void IncreaseHealth(int32 InHealValue);
	
protected:
	virtual void DeathProcessing(const FDamageData& InDamageData);
	
};
