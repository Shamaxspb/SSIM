// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMBaseEnemy.h"
#include "SSIMKhaimeraEnemy.generated.h"

UCLASS()
class SSIM_API ASSIMKhaimeraEnemy : public ASSIMBaseEnemy
{
	GENERATED_BODY()

public:
	ASSIMKhaimeraEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
