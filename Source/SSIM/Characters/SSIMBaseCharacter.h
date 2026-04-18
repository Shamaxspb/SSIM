// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SSIMBaseCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackFinishedSignature)

class UBoxComponent;


UCLASS(Abstract)
class SSIM_API ASSIMBaseCharacter : public ACharacter
{
	GENERATED_BODY()

// Variables
public:
	FOnAttackFinishedSignature OnAttackFinishedDelegate;

// Overriden Functions
public:
	ASSIMBaseCharacter();
	
	virtual void OnConstruction(const FTransform& Transform) override;		
	
// My Functions
private:
	void RestrictXAxisMovement() const;
};
