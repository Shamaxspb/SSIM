// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"

#include "SSIMPlayerDamageExecutionCalculation.generated.h"


UCLASS()
class SSIM_API USSIMPlayerDamageExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	USSIMPlayerDamageExecutionCalculation();
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, 
										FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
