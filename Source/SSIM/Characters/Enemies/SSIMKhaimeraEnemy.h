// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMBaseEnemy.h"

#include "SSIMKhaimeraEnemy.generated.h"

UCLASS()
class SSIM_API ASSIMKhaimeraEnemy : public ASSIMBaseEnemy
{
	GENERATED_BODY()

// Overriden Functions
public:
	ASSIMKhaimeraEnemy();
	
	virtual void BeginPlay() override;

};
