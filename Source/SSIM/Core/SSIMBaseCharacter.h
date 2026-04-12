// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SSIMBaseCharacter.generated.h"

UCLASS(Abstract)
class SSIM_API ASSIMBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASSIMBaseCharacter();
	
// My Functions
private:
	void RestrictXAxisMovement() const;
};
