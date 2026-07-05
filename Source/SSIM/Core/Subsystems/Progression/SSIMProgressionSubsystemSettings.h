// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SSIMProgressionSubsystemSettings.generated.h"

class UGameplayEffect;

UCLASS(Config = GameplaySubsystems, DefaultConfig, meta = (DisplayName = "Progression Subsystem Settings"))
class SSIM_API USSIMProgressionSubsystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, Category = "Progression Subsystem")
	TSubclassOf<UGameplayEffect> PlayerInitializationGE;
	
	UPROPERTY(Config, EditAnywhere, Category = "Progression Subsystem")
	TSubclassOf<UGameplayEffect> HealthLevelUpGE;
	UPROPERTY(Config, EditAnywhere, Category = "Progression Subsystem")
	TSubclassOf<UGameplayEffect> EnergyLevelUpGE;
	
	UPROPERTY(Config, EditAnywhere, Category = "Progression Subsystem")
	int32 PlayerBaseHealth;
	UPROPERTY(Config, EditAnywhere, Category = "Progression Subsystem")
	int32 PlayerBaseEnergy; 
	
	UPROPERTY(Config, EditAnywhere, Category = "Progression Subsystem")
	int32 PlayerMaximumHealth;
	UPROPERTY(Config, EditAnywhere, Category = "Progression Subsystem")
	int32 PlayerMaximumEnergy; 
	
	UPROPERTY(Config, EditAnywhere, Category = "Progression Subsystem")
	int32 HealthLevelUpShardsRequired;
	UPROPERTY(Config, EditAnywhere, Category = "Progression Subsystem")
	int32 EnergyLevelUpShardsRequired;
	
	UPROPERTY(Config, EditAnywhere, Category = "Progression Subsystem|Debug")
	bool bSaveOnEndPlay;
	
	
	virtual FName GetCategoryName() const override;
	virtual FText GetSectionText() const override;
	
};
