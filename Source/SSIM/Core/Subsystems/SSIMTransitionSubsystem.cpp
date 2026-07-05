// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMTransitionSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Progression/SSIMProgressionSubsystemSettings.h"
#include "SSIM/SSIM.h"
#include "SSIM/Characters/Player/SSIMPlayer.h"
#include "SSIM/Core/SSIMGameplayTags.h"
#include "SSIM/Core/GAS/SSIMCombatAttributeSet.h"

void USSIMTransitionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	LoadProgressionSubsystemSettings();
	
}

void USSIMTransitionSubsystem::Deinitialize()
{
	Super::Deinitialize();
	
	
}

void USSIMTransitionSubsystem::LoadProgressionSubsystemSettings()
{
	ProgressionSubsystemSettings = GetDefault<USSIMProgressionSubsystemSettings>();
	checkf(ProgressionSubsystemSettings, TEXT("%s | Failed to get Progression Subsystem Settings"), TEXT(__FUNCTION__));
}

void USSIMTransitionSubsystem::ApplyCurrentAttributesToPlayer()
{
	ASSIMPlayer* PlayerCharacter = Cast<ASSIMPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!IsValid(PlayerCharacter))
	{
		UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("%s | Player Character is not valid"), TEXT(__FUNCTION__));
		return;
	}
	
	const FGameplayEffectContextHandle EffectContext = PlayerCharacter->GetAbilitySystemComponent()->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = PlayerCharacter->GetAbilitySystemComponent()->MakeOutgoingSpec(
		ProgressionSubsystemSettings->InitPlayerCurrentAttributesGE,
		1.0f,
		EffectContext);
	
	// In case of loading first level 
	if (TransitionalHealth == 0 && TransitionalEnergy == 0)
	{
		TransitionalHealth = PlayerCharacter->GetAbilitySystemComponent()->GetNumericAttribute(USSIMCombatAttributeSet::GetMaxHealthAttribute());
		TransitionalEnergy = 3;
	}
	
	SpecHandle.Data->SetSetByCallerMagnitude(TAG_Attribute_Health, TransitionalHealth);
	SpecHandle.Data->SetSetByCallerMagnitude(TAG_Attribute_Energy, TransitionalEnergy);
	
	PlayerCharacter->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void USSIMTransitionSubsystem::CachePlayerCurrentAttributes()
{
	ASSIMPlayer* PlayerCharacter = Cast<ASSIMPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!IsValid(PlayerCharacter))
	{
		UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("%s | Player Character is not valid"), TEXT(__FUNCTION__));
		return;
	}
	
	TransitionalHealth = PlayerCharacter->GetAbilitySystemComponent()->GetNumericAttribute(USSIMCombatAttributeSet::GetHealthAttribute());
	TransitionalEnergy = PlayerCharacter->GetAbilitySystemComponent()->GetNumericAttribute(USSIMCombatAttributeSet::GetEnergyAttribute());
	
	UE_LOG(LogSSIMLevelTransition, Log, TEXT("%s | Cached Health on transition: %d"), TEXT(__FUNCTION__), TransitionalHealth);
	UE_LOG(LogSSIMLevelTransition, Log, TEXT("%s | Cached Energy on transition: %d"), TEXT(__FUNCTION__), TransitionalEnergy);
}
