// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EPlayerState.generated.h"



UENUM(BlueprintType)
enum class EPlayerState : uint8
{
		EPS_Movement	UMETA(DisplayName = "Movement"),
		EPS_Climb		UMETA(DisplayName = "Climb"),
		EPS_Attack		UMETA(DisplayName = "Attack"),
		EPS_Dodge		UMETA(DisplayName = "Dodge"),
		EPS_Hit			UMETA(DisplayName = "Hit")
};
