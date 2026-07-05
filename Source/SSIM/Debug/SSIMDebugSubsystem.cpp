// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMDebugSubsystem.h"

#include "SSIM/SSIM.h"
#include "SSIM/Core/Subsystems/Progression/SSIMProgressionSubsystem.h"

void USSIMDebugSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	Collection.InitializeDependency<USSIMProgressionSubsystem>();
	
	ProgressionSubsystem = GetGameInstance()->GetSubsystem<USSIMProgressionSubsystem>();
	checkf(ProgressionSubsystem, TEXT("%s | Progression Subsystem is NULL"), TEXT(__FUNCTION__));
	UE_LOG(LogSSIMDebug, Log, TEXT("%s | Progression Subsystem loaded"), TEXT(__FUNCTION__));
	
	RegisterDebugCommands();
}

void USSIMDebugSubsystem::Deinitialize()
{
	Super::Deinitialize();
	
	UnregisterDebugCommands();
}

void USSIMDebugSubsystem::RegisterDebugCommands() const
{
	// Register Add Health Shard
	IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("dbg.AddHealthShard"),
		TEXT("Add 1 Health Shard"),
		FConsoleCommandDelegate::CreateUObject(this, &USSIMDebugSubsystem::AddHealthShard),
		ECVF_Default);
	
	// Register Add Health Shards
	IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("dbg.AddHealthShards"),
		TEXT("Add N Health Shards"),
		FConsoleCommandWithArgsDelegate::CreateUObject(this, &USSIMDebugSubsystem::HandleAddHealthShards),
		ECVF_Default);
	
	// Register Add Energy Shard
	IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("dbg.AddEnergyShard"),
		TEXT("Add 1 Energy Shard"),
		FConsoleCommandDelegate::CreateUObject(this, &USSIMDebugSubsystem::AddEnergyShard),
		ECVF_Default);
	
	// Register Add Energy Shards
	IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("dbg.AddEnergyShards"),
		TEXT("Add N Energy Shards"),
		FConsoleCommandWithArgsDelegate::CreateUObject(this, &USSIMDebugSubsystem::HandleAddEnergyShards),
		ECVF_Default);
	
	// Register Save Progression
	IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("dbg.SaveProgression"),
		TEXT("Serializes ProgressionSubsystem to ProgressionSaveData"),
		FConsoleCommandDelegate::CreateUObject(this, &USSIMDebugSubsystem::SaveProgressionData),
		ECVF_Default);
}

void USSIMDebugSubsystem::UnregisterDebugCommands() const
{
	IConsoleManager::Get().UnregisterConsoleObject(TEXT("dbg.AddHealthShard"), false);
	IConsoleManager::Get().UnregisterConsoleObject(TEXT("dbg.AddHealthShards"), false);
	IConsoleManager::Get().UnregisterConsoleObject(TEXT("dbg.AddEnergyShard"), false);
	IConsoleManager::Get().UnregisterConsoleObject(TEXT("dbg.AddEnergyShards"), false);
	IConsoleManager::Get().UnregisterConsoleObject(TEXT("dbg.SaveProgression"), false);
}

void USSIMDebugSubsystem::AddHealthShard() const
{
	ProgressionSubsystem->AddHealthShard();
}

void USSIMDebugSubsystem::HandleAddHealthShards(const TArray<FString>& Args) const
{
	if (Args.Num() < 1)
	{
		UE_LOG(LogSSIMDebug, Error, TEXT("Correct usage: dbg.AddHealthShards [ShardAmount]"));
		return;
	}
	
	const int32 ShardAmount = FCString::Atoi(*Args[0]);
	
	AddHealthShards(ShardAmount);
}

void USSIMDebugSubsystem::AddHealthShards(int32 InShardAmount) const
{
	for (int32 i = 0; i < InShardAmount; ++i)
	{
		ProgressionSubsystem->AddHealthShard();
	}
}

void USSIMDebugSubsystem::AddEnergyShard() const
{
	ProgressionSubsystem->AddEnergyShard();
}

void USSIMDebugSubsystem::HandleAddEnergyShards(const TArray<FString>& Args) const
{
	if (Args.Num() < 1)
	{
		UE_LOG(LogSSIMDebug, Error, TEXT("Correct usage: dbg.AddEnergyShards [ShardAmount]"));
		return;
	}
	
	const int32 ShardAmount = FCString::Atoi(*Args[0]);
	
	AddEnergyShards(ShardAmount);
}

void USSIMDebugSubsystem::AddEnergyShards(int32 InShardAmount) const
{
	for (int32 i = 0; i < InShardAmount; ++i)
	{
		ProgressionSubsystem->AddEnergyShard();
	}
}

void USSIMDebugSubsystem::SaveProgressionData() const
{
	ProgressionSubsystem->SerializeSaveData();
}


