// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "SSIMDebugSubsystem.generated.h"


class USSIMProgressionSubsystemSettings;
class USSIMProgressionSubsystem;

UCLASS(meta = (DisplayName = "Debug Subsystem"))
class SSIM_API USSIMDebugSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
// Variables
	UPROPERTY()
	TObjectPtr<USSIMProgressionSubsystem> ProgressionSubsystem;
	UPROPERTY()
	const USSIMProgressionSubsystemSettings* ProgressionSubsystemSettings;
	
// Overriden Functions
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
// Custom Functions
private:
	void RegisterDebugCommands() const;
	void UnregisterDebugCommands() const;
	
	void ListAllProgressionData() const;
	
	void AddHealthShard() const;
	
	void HandleAddHealthShards(const TArray<FString>& Args) const;
	void AddHealthShards(int32 InShardAmount) const;
	
	void AddEnergyShard() const;
	
	void HandleAddEnergyShards(const TArray<FString>& Args) const;
	void AddEnergyShards(int32 InShardAmount) const;
	
	void SaveProgressionData() const;
};
