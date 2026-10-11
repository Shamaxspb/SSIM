// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMCombatAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "SSIM/SSIM.h"
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
	/*else if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Clamp(
			NewValue, 
			1.f, 
			ProgressionSettings->AbsoluteMaximumHealth);
	}*/
	else if (Attribute == GetMaxEnergyAttribute())
	{
		// Since Energy is Player only related attribute
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
	const float OldHealth = GetHealth();
	
	SetDamage(0.f);
	
	if (LocalDamage <= 0.f)
	{
		return;
	}
	
	SetHealth(OldHealth - LocalDamage);
	
	const float NewHealth = GetHealth();
	
	
	if (GetHealth() <= 0.f)
	{
		HandleDeath(Data);
	}
	
	UE_LOG(LogSSIMStatsCalculation, Log, TEXT("%s | %s | %f damage taken. HP %f -> %f"),
										 TEXT(__FUNCTION__),
										 *GetOwningActor()->GetName(),
										 LocalDamage,
										 OldHealth,
										 NewHealth);
}



void USSIMCombatAttributeSet::HandleHeal(const FGameplayEffectModCallbackData& Data)
{
	const float LocalHeal = GetHeal();
	const float OldHealth = GetHealth();
	
	SetHeal(0.f);
	
	if (LocalHeal <= 0.f)
	{
		return;
	}
	
	SetHealth(OldHealth + LocalHeal);
	
	const float NewHealth = GetHealth();
	
	UE_LOG(LogSSIMStatsCalculation, Log, TEXT("%s | %s | %f heal received. HP %f -> %f"),
										 TEXT(__FUNCTION__),
										 *GetOwningActor()->GetName(),
										 LocalHeal,
										 OldHealth,
										 NewHealth);
}

void USSIMCombatAttributeSet::HandleDeath(const FGameplayEffectModCallbackData& Data)
{
	// Not sure about signature
	UE_LOG(LogSSIMStatsCalculation, Warning, TEXT("%s | Death Processing is not implemented"), TEXT(__FUNCTION__));
}