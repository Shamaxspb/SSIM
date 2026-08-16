// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerDamageExecutionCalculation.h"

#include "SSIM/Core/GAS/SSIMCombatAttributeSet.h"


struct FDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);
	
	FDamageCapture()
	{
		// Source
		DEFINE_ATTRIBUTE_CAPTUREDEF(USSIMCombatAttributeSet, Damage, Source, true);
	}
};

static const FDamageCapture& GetDamageCapture()
{
	static FDamageCapture DamageCapture;
	return DamageCapture;
}

USSIMPlayerDamageExecutionCalculation::USSIMPlayerDamageExecutionCalculation()
{
	RelevantAttributesToCapture.Add(GetDamageCapture().DamageDef);
}


void USSIMPlayerDamageExecutionCalculation::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
	
	
}
