// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMCombatDataTypes.generated.h"

UENUM(BlueprintType)
enum class EPlayerAttackDirectionType : uint8
{
	EPADT_Default	UMETA(Hidden),
	EPADT_Frontal	UMETA(DisplayName = "Frontal"),
	EPADT_Upward 	UMETA(DisplayName = "Upward"),
	EPADT_Downward	UMETA(DisplayName = "Downward")
};

USTRUCT(Blueprintable)
struct FDamageData
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> Instigator = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	int32 Value = 0;
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Default		UMETA(Hidden),
	EES_Idle		UMETA(DisplayName = "Idle"),
	EES_Combat 		UMETA(DisplayName = "Combat"),
	EES_Staggered	UMETA(DisplayName = "Staggered"),
	EES_Dead		UMETA(DisplayName = "Dead")
};
