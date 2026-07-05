// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"

#include "SSIMDamageExecutionCalculation.generated.h"


UCLASS()
class SSIM_API USSIMDamageExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
	// This class is overengineering for now, to complex for my project
	
public:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, 
										FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
