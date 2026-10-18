// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "SSIMCombatSubsystemSettings.generated.h"


class UGameplayEffect;

UCLASS(Config = GameplaySubsystems, DefaultConfig, meta = (DisplayName = "Progression Subsystem Settings"))
class SSIM_API USSIMCombatSubsystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, Category = "Combat Subsystem")
	TSubclassOf<UGameplayEffect> DamageGE;
	
	UPROPERTY(Config, EditAnywhere, Category = "Combat Subsystem")
	TSubclassOf<UGameplayEffect> HealGE;
	
	UPROPERTY(Config, EditAnywhere, Category = "Combat Subsystem")
	TSubclassOf<UGameplayEffect> IncreaseEnergyGE;
	
	
	virtual FName GetCategoryName() const override;
	virtual FText GetSectionText() const override;
};
