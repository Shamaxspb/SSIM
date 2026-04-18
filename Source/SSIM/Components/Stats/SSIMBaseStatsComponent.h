// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIM/Components/SSIMBaseComponent.h"
#include "SSIMBaseStatsComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SSIM_API USSIMBaseStatsComponent : public USSIMBaseComponent
{
	GENERATED_BODY()

	
// My Functions
public:
	virtual void ReduceHealth();
};
