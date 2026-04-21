// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMCombatDataTypes.generated.h"

UENUM(Blueprintable)
enum class EPlayerAttackDirection : uint8
{
	EPAD_Default	UMETA(Hidden),
	EPAD_Frontal	UMETA(DisplayName = "Frontal"),
	EPAD_Upward 	UMETA(DisplayName = "Upward"),
	EPAD_Downward	UMETA(DisplayName = "Downward")
};

USTRUCT(Blueprintable)
struct FDamageData
{
	GENERATED_BODY()
	
	UPROPERTY()
	TObjectPtr<AActor> DamageInstigator;
	
	int32 DamageValue;
};