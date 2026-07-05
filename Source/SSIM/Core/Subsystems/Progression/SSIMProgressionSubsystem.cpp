// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMProgressionSubsystem.h"

#include "SSIMProgressionSaveData.h"
#include "SSIMProgressionSubsystemSettings.h"
#include "Kismet/GameplayStatics.h"
#include "SSIM/SSIM.h"
#include "SSIM/Characters/Player/SSIMPlayer.h"
#include "SSIM/Core/SSIMGameplayTags.h"

void USSIMProgressionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	LoadProgressionSubsystemSettings();
	LoadProgressionSave();
}

void USSIMProgressionSubsystem::Deinitialize()
{
	Super::Deinitialize();
	
	if (ProgressionSubsystemSettings->bSaveOnEndPlay)
	{
		SerializeSaveData();
	}
}

void USSIMProgressionSubsystem::ApplyProgressionToPlayer()
{
	ASSIMPlayer* PlayerCharacter = Cast<ASSIMPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!IsValid(PlayerCharacter))
	{
		UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("%s | Player Character is not valid"), TEXT(__FUNCTION__));
		return;
	}
	
	const FGameplayEffectContextHandle EffectContext = PlayerCharacter->GetAbilitySystemComponent()->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = PlayerCharacter->GetAbilitySystemComponent()->MakeOutgoingSpec(
		ProgressionSubsystemSettings->PlayerInitializationGE,
		1.0f,
		EffectContext);
	
	if (!SpecHandle.IsValid())
	{
		UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("%s | SpecHandle is not valid"), TEXT(__FUNCTION__));
		return;
	}
	
	FPlayerInitAttributes Attributes = CalculatePlayerInitAttributes();
	
	SpecHandle.Data->SetSetByCallerMagnitude(TAG_Attribute_Health,	 Attributes.Health);
	SpecHandle.Data->SetSetByCallerMagnitude(TAG_Attribute_MaxHealth, Attributes.MaxHealth);
	SpecHandle.Data->SetSetByCallerMagnitude(TAG_Attribute_Energy,	 Attributes.Energy);
	SpecHandle.Data->SetSetByCallerMagnitude(TAG_Attribute_MaxEnergy, Attributes.MaxEnergy);
	
	PlayerCharacter->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	
	UE_LOG(LogSSIMPlayerInitialization, Log, TEXT("%s | Total Player Health: %d"), TEXT(__FUNCTION__), Attributes.MaxHealth);
	UE_LOG(LogSSIMProgression, Log, TEXT("%s | Remaining Health Shards: %d/%d"), TEXT(__FUNCTION__), 
		RemainingHealthShards, ProgressionSubsystemSettings->HealthLevelUpShardsRequired);
	UE_LOG(LogSSIMPlayerInitialization, Log, TEXT("%s | Total Player Energy: %d"), TEXT(__FUNCTION__), Attributes.MaxEnergy);
	UE_LOG(LogSSIMProgression, Log, TEXT("%s | Remaining Energy Shards: %d/%d"), TEXT(__FUNCTION__), 
		RemainingEnergyShards, ProgressionSubsystemSettings->EnergyLevelUpShardsRequired);
}

void USSIMProgressionSubsystem::AddHealthShard()
{
	CollectedHealthShards++;
	RemainingHealthShards++;
	
	UE_LOG(LogSSIMProgression, Log, TEXT("%s | Health Shard added. Remaining Health Shards: %d/%d"), TEXT(__FUNCTION__), 
		RemainingHealthShards, ProgressionSubsystemSettings->HealthLevelUpShardsRequired);
	
	if (RemainingHealthShards >= ProgressionSubsystemSettings->HealthLevelUpShardsRequired)
	{
		ASSIMPlayer* PlayerCharacter = Cast<ASSIMPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (!IsValid(PlayerCharacter))
		{
			UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("%s | Player Character is not valid"), TEXT(__FUNCTION__));
			return;
		}
		
		const FGameplayEffectContextHandle EffectContext = PlayerCharacter->GetAbilitySystemComponent()->MakeEffectContext();
	
		const FGameplayEffectSpecHandle SpecHandle = PlayerCharacter->GetAbilitySystemComponent()->MakeOutgoingSpec(
			ProgressionSubsystemSettings->HealthLevelUpGE,
			1.0f,
			EffectContext);
	
		if (!SpecHandle.IsValid())
		{
			UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("%s | SpecHandle is not valid"), TEXT(__FUNCTION__));
			return;
		}
		
		PlayerCharacter->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		
		RemainingHealthShards = 0;
		
		UE_LOG(LogSSIMProgression, Warning, TEXT("%s | Max Health Increased"), TEXT(__FUNCTION__));
	}
}

void USSIMProgressionSubsystem::AddEnergyShard()
{
	CollectedEnergyShards++;
	RemainingEnergyShards++;
	
	UE_LOG(LogSSIMProgression, Log, TEXT("%s | Energy Shard added. Remaining Energy Shards: %d/%d"), TEXT(__FUNCTION__), 
		RemainingEnergyShards, ProgressionSubsystemSettings->EnergyLevelUpShardsRequired);
	
	if (RemainingEnergyShards >= ProgressionSubsystemSettings->EnergyLevelUpShardsRequired)
	{
		ASSIMPlayer* PlayerCharacter = Cast<ASSIMPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (!IsValid(PlayerCharacter))
		{
			UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("%s | Player Character is not valid"), TEXT(__FUNCTION__));
			return;
		}
		
		const FGameplayEffectContextHandle EffectContext = PlayerCharacter->GetAbilitySystemComponent()->MakeEffectContext();
	
		const FGameplayEffectSpecHandle SpecHandle = PlayerCharacter->GetAbilitySystemComponent()->MakeOutgoingSpec(
			ProgressionSubsystemSettings->EnergyLevelUpGE,
			1.0f,
			EffectContext);
	
		if (!SpecHandle.IsValid())
		{
			UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("%s | SpecHandle is not valid"), TEXT(__FUNCTION__));
			return;
		}
		
		PlayerCharacter->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		
		RemainingEnergyShards = 0;
		
		UE_LOG(LogSSIMProgression, Warning, TEXT("%s | Max Energy Increased"), TEXT(__FUNCTION__));
	}
}

void USSIMProgressionSubsystem::LoadProgressionSubsystemSettings()
{
	ProgressionSubsystemSettings = GetDefault<USSIMProgressionSubsystemSettings>();
	
	checkf(ProgressionSubsystemSettings, TEXT("%s | Failed to get Progression Subsystem Settings"), TEXT(__FUNCTION__));
	
}

void USSIMProgressionSubsystem::CreateDefaultProgressionSave()
{
	ProgressionSaveData = Cast<USSIMProgressionSaveData>(
		UGameplayStatics::CreateSaveGameObject(USSIMProgressionSaveData::StaticClass()));
	checkf(ProgressionSaveData, TEXT("%s | Failed to Create Progression Save"), TEXT(__FUNCTION__));
	
	UGameplayStatics::SaveGameToSlot(ProgressionSaveData, ProgressionDataSlotName, 0);
}

void USSIMProgressionSubsystem::LoadProgressionSave()
{
	if (UGameplayStatics::DoesSaveGameExist(ProgressionDataSlotName, 0))
	{
		USaveGame* LoadedProgressionSaveData = UGameplayStatics::LoadGameFromSlot(ProgressionDataSlotName, 0);
		checkf(LoadedProgressionSaveData, TEXT("LoadedProgressionSaveData returned nullptr"));
		
		ProgressionSaveData = Cast<USSIMProgressionSaveData>(LoadedProgressionSaveData);
		checkf(ProgressionSaveData, TEXT("Cast Failed: Loaded save is not UKTProgressionSaveData"));
	
		UE_LOG(LogSSIMPlayerInitialization, Log, TEXT("%s | Progression Save Data Loaded"), TEXT(__FUNCTION__));
		
		if (!IsValid(ProgressionSaveData))
		{
			CreateDefaultProgressionSave();
			UE_LOG(LogSSIMPlayerInitialization, Warning, 
				TEXT("%s | Progression Save Data is not valid. New Progression Save created"), TEXT(__FUNCTION__));
		}
	}
	else
	{
		CreateDefaultProgressionSave();
		UE_LOG(LogSSIMPlayerInitialization, Warning, 
				TEXT("%s | Progression Save Data does not exist. New Progression Save created"), TEXT(__FUNCTION__));
	}
	
	DeserializeSaveData();
}

void USSIMProgressionSubsystem::SerializeSaveData() const
{
	if (!ProgressionSaveData)
	{
		UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("%s | Progression Save Data is not valid"), TEXT(__FUNCTION__));
		return;
	}
	
	ProgressionSaveData->CollectedHealthShards = CollectedHealthShards;
	ProgressionSaveData->CollectedEnergyShards = CollectedEnergyShards;
	
	UE_LOG(LogSSIMProgression, Log, TEXT("%s | Progression Data serialized"), TEXT(__FUNCTION__));
}

void USSIMProgressionSubsystem::DeserializeSaveData()
{
	if (!ProgressionSaveData)
	{
		UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("%s | Progression Save Data is not valid"), TEXT(__FUNCTION__));
		return;
	}
	
	CollectedHealthShards = ProgressionSaveData->CollectedHealthShards;
	CollectedEnergyShards = ProgressionSaveData->CollectedEnergyShards;
}

FPlayerInitAttributes USSIMProgressionSubsystem::CalculatePlayerInitAttributes()
{
	FPlayerInitAttributes Attributes;
	
	int32 AdditionalHealth = 
		ProgressionSaveData->CollectedHealthShards / ProgressionSubsystemSettings->HealthLevelUpShardsRequired;
	
	RemainingHealthShards = 
		ProgressionSaveData->CollectedHealthShards % ProgressionSubsystemSettings->HealthLevelUpShardsRequired;
	
	int32 AdditionalEnergy = 
		ProgressionSaveData->CollectedEnergyShards / ProgressionSubsystemSettings->EnergyLevelUpShardsRequired;
	
	RemainingEnergyShards = 
		ProgressionSaveData->CollectedEnergyShards % ProgressionSubsystemSettings->EnergyLevelUpShardsRequired;
	
	Attributes.MaxHealth = ProgressionSubsystemSettings->PlayerBaseHealth + AdditionalHealth;
	Attributes.Health = Attributes.MaxHealth;
	Attributes.MaxEnergy = ProgressionSubsystemSettings->PlayerBaseEnergy + AdditionalEnergy;
	Attributes.Energy = 3;
	
	UE_LOG(LogSSIMStatsCalculation, Log, TEXT("%s | AdditionalHealth: %d"), TEXT(__FUNCTION__), AdditionalHealth);
	UE_LOG(LogSSIMStatsCalculation, Log, TEXT("%s | RemainingHealthShards: %d"), TEXT(__FUNCTION__), RemainingHealthShards);
	UE_LOG(LogSSIMStatsCalculation, Log, TEXT("%s | AdditionalEnergy: %d"), TEXT(__FUNCTION__), AdditionalEnergy);
	UE_LOG(LogSSIMStatsCalculation, Log, TEXT("%s | RemainingEnergyShards: %d"), TEXT(__FUNCTION__), RemainingEnergyShards);
	
	return Attributes;
}
