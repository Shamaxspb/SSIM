// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMBaseUI.h"
#include "SSIMHealthBar.generated.h"

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
	float Health;
	float MaxHealth;

private:
	UPROPERTY()
	TObjectPtr<USSIMPlayerStatsComponent> PlayerStatsComponent;
	
	
	// My Functions
protected:	
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged();

private:
	void Init();
	virtual void SetReferences() override;
	void InitStatsValues();
};
