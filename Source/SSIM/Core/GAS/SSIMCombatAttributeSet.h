// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"

#include "SSIMCombatAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName) \


UCLASS()
class SSIM_API USSIMCombatAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	/* META */
	ATTRIBUTE_ACCESSORS(USSIMCombatAttributeSet, Damage);
	ATTRIBUTE_ACCESSORS(USSIMCombatAttributeSet, Heal);
	
	ATTRIBUTE_ACCESSORS(USSIMCombatAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(USSIMCombatAttributeSet, MaxHealth);
	
	ATTRIBUTE_ACCESSORS(USSIMCombatAttributeSet, Energy);
	ATTRIBUTE_ACCESSORS(USSIMCombatAttributeSet, MaxEnergy);
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SSIM|Combat")
	FGameplayAttributeData Damage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SSIM|Combat")
	FGameplayAttributeData Heal;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SSIM|Combat")
	FGameplayAttributeData Health;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SSIM|Combat")
	FGameplayAttributeData MaxHealth;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SSIM|Combat")
	FGameplayAttributeData Energy;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SSIM|Combat")
	FGameplayAttributeData MaxEnergy;
	
// Overriden Functions
public:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	
// Custom Functions
	void HandleDamage(const FGameplayEffectModCallbackData& Data);
	void HandleHeal(const FGameplayEffectModCallbackData& Data);
};
