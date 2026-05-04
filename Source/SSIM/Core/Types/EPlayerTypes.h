// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EPlayerTypes.generated.h"

/*UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	EPS_Movement	UMETA(DisplayName = "Movement"),
	EPS_Climb		UMETA(DisplayName = "Climb"),
	EPS_Attack		UMETA(DisplayName = "Attack"),
	EPS_Dodge		UMETA(DisplayName = "Dodge"),
	EPS_Hit			UMETA(DisplayName = "Hit")
};*/

UENUM(BlueprintType)
enum class EFacingDirection : uint8
{
	EPD_Right		UMETA(DisplayName = "Right"),
	EPD_Left		UMETA(DisplayName = "Left")
};
