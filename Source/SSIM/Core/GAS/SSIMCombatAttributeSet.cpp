// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMCombatAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "SSIM/Core/Subsystems/Progression/SSIMProgressionSubsystemSettings.h"

void USSIMCombatAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	const USSIMProgressionSubsystemSettings* ProgressionSettings = GetDefault<USSIMProgressionSubsystemSettings>();
	
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetEnergyAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxEnergy());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Clamp(
			NewValue, 
			1.f, 
			ProgressionSettings->AbsoluteMaximumHealth);
	}
	else if (Attribute == GetMaxEnergyAttribute())
	{
		NewValue = FMath::Clamp(
			NewValue, 
			1.f, 
			ProgressionSettings->AbsoluteMaximumEnergy);
	}
}

void USSIMCombatAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		HandleDamage(Data);
	}
	else if (Data.EvaluatedData.Attribute == GetHealAttribute())
	{
		HandleHeal(Data);
	}
}

void USSIMCombatAttributeSet::HandleDamage(const FGameplayEffectModCallbackData& Data)
{
	const float LocalDamage = GetDamage();
	
	SetDamage(0.f);
	
	if (LocalDamage <= 0.f)
	{
		return;
	}
	
	SetHealth(GetHealth() - LocalDamage);
	
	if (GetHealth() <= 0.f)
	{
		// Owner died
	}
}

void USSIMCombatAttributeSet::HandleHeal(const FGameplayEffectModCallbackData& Data)
{
	const float LocalHeal = GetHeal();
	
	SetHeal(0.f);
	
	if (LocalHeal <= 0.f)
	{
		return;
	}
	
	SetHealth(GetHealth() + LocalHeal);
}
