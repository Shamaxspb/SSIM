// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(Blueprintable)
enum class EPlayerAttackDirection : uint8
{
	EPAD_Default	UMETA(Hidden),
	EPAD_Frontal	UMETA(DisplayName = "Frontal"),
	EPAD_Upward 	UMETA(DisplayName = "Upward"),
	EPAD_Downward	UMETA(DisplayName = "Downward")
};
