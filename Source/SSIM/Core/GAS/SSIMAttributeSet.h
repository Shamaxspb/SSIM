// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "SSIMAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class SSIM_API USSIMAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
	int Health;
	int MaxHealth;
	
	int Energy;
	int MaxEnergy;
};
