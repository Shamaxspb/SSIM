// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMBaseUI.h"
#include "SSIMHealthBar.generated.h"

struct FDamageData;
class ASSIMPlayer;
class USSIMPlayerStatsComponent;
/**
 * 
 */
UCLASS(NotBlueprintable)
class SSIM_API USSIMHealthBar : public USSIMBaseUI
{
	GENERATED_BODY()
	
// Variables
protected:
	UPROPERTY(BlueprintReadWrite, Category = "SSIM|HealthBar")
	int32 CachedHealth;
	int32 MaxHealth;

private:
	UPROPERTY()
	TObjectPtr<USSIMPlayerStatsComponent> PlayerStatsComponent;
	
	
// Overriden Functions
protected:
	virtual void NativeConstruct() override;
	
	
// My Functions
protected:	
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(const FDamageData& InDamageData);
	
	virtual void SetReferences() override;

private:
	void Init();
	void InitStatsValues();
	
	UFUNCTION()
	void OnDamageReceived(const FDamageData& InDamageData);
	
	UFUNCTION()
	void OnHealReceived(const FDamageData& InDamageData);
};
