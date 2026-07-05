// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "SSIMProgressionSubsystem.generated.h"


class USSIMProgressionSaveData;
class USSIMProgressionSubsystemSettings;


struct FPlayerInitAttributes
{
	int32 Health;
	int32 MaxHealth;
	int32 Energy;
	int32 MaxEnergy;
};


UCLASS(meta = (DisplayName = "Progression Subsystem"))
class SSIM_API USSIMProgressionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
// Variables
public:
	UPROPERTY(BlueprintReadOnly, Category = "SSIM|Progression")
	int32 CollectedHealthShards;
	UPROPERTY(BlueprintReadOnly, Category = "SSIM|Progression")
	int32 CollectedEnergyShards;
	
	UPROPERTY(BlueprintReadOnly, Category = "SSIM|Progression")
	int32 RemainingHealthShards;
	UPROPERTY(BlueprintReadOnly, Category = "SSIM|Progression")
	int32 RemainingEnergyShards;
	
private:
	UPROPERTY()
	const USSIMProgressionSubsystemSettings* ProgressionSubsystemSettings;
	
	UPROPERTY()
	TObjectPtr<USSIMProgressionSaveData> ProgressionSaveData;
	const TCHAR* ProgressionDataSlotName = TEXT("ProgressionDataSlot");
	
// Overriden Functions
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual void Deinitialize() override;
	
// Custom Functions
public:
	void ApplyProgressionToPlayer();
	
	void SerializeSaveData() const;
	void DeserializeSaveData();
	
	void AddHealthShard();
	void AddEnergyShard();
	
private:
	void LoadProgressionSubsystemSettings();
	void CreateDefaultProgressionSave();
	void LoadProgressionSave();
	
	FPlayerInitAttributes CalculatePlayerInitAttributes();
	
};
