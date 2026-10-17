// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMCombatAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "SSIM/SSIM.h"
#include "SSIM/Core/Subsystems/Progression/SSIMProgressionSubsystemSettings.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "SSIM/Core/SSIMGameplayTags.h"

void USSIMCombatAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetEnergyAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxEnergy());
		UE_LOG(LogSSIMStatsCalculation, Log, TEXT(""));
	}
						// this is commented since there is only one common attribute set and 
						// MaxHealth() should be clamped by AbsoluteMaximumHealth only for Player
						//const USSIMProgressionSubsystemSettings* ProgressionSettings = GetDefault<USSIMProgressionSubsystemSettings>();
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
		const USSIMProgressionSubsystemSettings* ProgressionSettings = GetDefault<USSIMProgressionSubsystemSettings>();
		NewValue = FMath::Clamp(NewValue, 1.f, static_cast<float>(ProgressionSettings->AbsoluteMaximumEnergy));
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

void USSIMCombatAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute,float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if (Attribute == GetEnergyAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxEnergy());
	}
	else if (Attribute == GetMaxEnergyAttribute())
	{
		const USSIMProgressionSubsystemSettings* Settings =	GetDefault<USSIMProgressionSubsystemSettings>();
		NewValue = FMath::Clamp(NewValue,1.f,static_cast<float>(Settings->AbsoluteMaximumEnergy));
	}
}

void USSIMCombatAttributeSet::HandleDamage(const FGameplayEffectModCallbackData& Data)
{
	UAbilitySystemComponent* OwnerASC = GetOwningAbilitySystemComponent();
	if (!IsValid(OwnerASC))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s OwnerASC is not valid"), TEXT(__FUNCTION__));
		return;
	}
	
	const float LocalDamage = GetDamage();
	SetDamage(0.f);
	
	if (LocalDamage <= 0.f)
	{
		return;
	}
	
	const float OldHealth = GetHealth();
	const float NewHealth = FMath::Clamp(OldHealth - LocalDamage, 0.f, GetMaxHealth());
	const float ActualDamage = OldHealth - NewHealth;
	
	SetHealth(NewHealth);
	
	FGameplayEventData Payload;
	Payload.EventTag = TAG_Event_DamageTaken.GetTag();
	Payload.Instigator = Data.EffectSpec.GetContext().GetInstigator();
	Payload.Target = GetOwningActor();
	Payload.ContextHandle = Data.EffectSpec.GetContext();
	Payload.EventMagnitude = ActualDamage;
	
	OwnerASC->HandleGameplayEvent(TAG_Event_DamageTaken, &Payload);
	
	if (GetHealth() <= 0.f)
	{
		HandleDeath(Data);
	}
	
	UE_LOG(LogSSIMStatsCalculation, Log, TEXT("%s | %s | %f damage taken. HP %f -> %f"),
										 TEXT(__FUNCTION__),
										 *GetOwningActor()->GetName(),
										 ActualDamage,
										 OldHealth,
										 NewHealth);
}

void USSIMCombatAttributeSet::HandleHeal(const FGameplayEffectModCallbackData& Data)
{
	const float LocalHeal = GetHeal();
	SetHeal(0.f);
	
	if (LocalHeal <= 0.f)
	{
		return;
	}
	
	const float OldHealth = GetHealth();
	const float NewHealth = FMath::Clamp(OldHealth + LocalHeal, 0.f, GetMaxHealth());
	const float ActualHeal = NewHealth - OldHealth;
	
	SetHealth(NewHealth);
	
	FGameplayEventData Payload;
	Payload.EventTag = TAG_Event_Heal_Received.GetTag();
	Payload.Instigator = GetOwningActor();
	Payload.Target = GetOwningActor();
	Payload.ContextHandle = Data.EffectSpec.GetContext();
	Payload.EventMagnitude = ActualHeal;
	
	GetOwningAbilitySystemComponent()->HandleGameplayEvent(TAG_Event_Heal_Received, &Payload);
	
	UE_LOG(LogSSIMStatsCalculation, Log, TEXT("%s | %s | %f heal received. HP %f -> %f"),
										 TEXT(__FUNCTION__),
										 *GetOwningActor()->GetName(),
										 ActualHeal,
										 OldHealth,
										 NewHealth);
}

void USSIMCombatAttributeSet::HandleDeath(const FGameplayEffectModCallbackData& Data)
{
	// Not sure about signature
	UE_LOG(LogSSIMStatsCalculation, Warning, TEXT("%s | Death Processing is not implemented"), TEXT(__FUNCTION__));
}
