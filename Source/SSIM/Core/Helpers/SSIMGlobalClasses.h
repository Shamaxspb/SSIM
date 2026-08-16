// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SSIMGlobalClasses.generated.h"

class UGameplayEffect;
/**
 * 
 */
UCLASS(Config = GameplaySubsystems, DefaultConfig, meta = (DisplayName = "SSIM Global Classes"))
class SSIM_API USSIMGlobalClasses : public UDeveloperSettings
{
	GENERATED_BODY()
	
	// Variables
public:
	UPROPERTY(Config, EditAnywhere, Category = "Global Classes|Damage", meta = (DisplayName = "GE_PlayerDamage"))
	TSubclassOf<UGameplayEffect> PlayerDamageGE;
	
	UPROPERTY(Config, EditAnywhere, Category = "Global Classes|Damage", meta = (DisplayName = "GE_EnemyDamage"))
	TSubclassOf<UGameplayEffect> EnemyDamageGE;
	
	
	// Overriden Functions
	virtual FName GetCategoryName() const override;
	
#if WITH_EDITOR
	virtual FText GetSectionText() const override;
#endif WITH_EDITOR
};
