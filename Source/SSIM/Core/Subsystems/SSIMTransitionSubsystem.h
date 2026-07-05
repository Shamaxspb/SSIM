// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SSIMTransitionSubsystem.generated.h"

class USSIMProgressionSubsystemSettings;


struct FPlayerCurrentAttributest
{
	int32 CurrentHealth;
	int32 CurrentEnergy;
};

UCLASS()
class SSIM_API USSIMTransitionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	// Variables
public:
	UPROPERTY(BlueprintReadOnly, Category = "SSIM|Progression")
	int32 TransitionalHealth = 0;
	UPROPERTY(BlueprintReadOnly, Category = "SSIM|Progression")
	int32 TransitionalEnergy = 0;
	
private:
	UPROPERTY()
	const USSIMProgressionSubsystemSettings* ProgressionSubsystemSettings;
	
	FPlayerCurrentAttributest CurrentAttributes = {0, 0};
	
// Overriden Functions
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	

	// Custom Functions
public:
	void ApplyCurrentAttributesToPlayer();
	void CachePlayerCurrentAttributes();
	
private:
	void LoadProgressionSubsystemSettings();
};
