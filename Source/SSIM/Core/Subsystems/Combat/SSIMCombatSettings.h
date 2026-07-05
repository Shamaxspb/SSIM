// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "SSIMCombatSettings.generated.h"


class UGameplayEffect;

UCLASS(Config = GameplaySubsystems, DefaultConfig, meta = (DisplayName = "Progression Subsystem Settings"))
class SSIM_API USSIMCombatSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, Category = "Combat Subsystem")
	TSubclassOf<UGameplayEffect> DamageGE;
	
	UPROPERTY(Config, EditAnywhere, Category = "Combat Subsystem")
	TSubclassOf<UGameplayEffect> HealGE;
	
	
	virtual FName GetCategoryName() const override;
	virtual FText GetSectionText() const override;
};
