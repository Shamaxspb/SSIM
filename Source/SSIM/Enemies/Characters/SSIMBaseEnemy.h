// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIM/Core/SSIMBaseCharacter.h"
#include "SSIMBaseEnemy.generated.h"

class USSIMEnemyHealthComponent;
class UBehaviorTreeComponent;

UCLASS(Abstract)
class SSIM_API ASSIMBaseEnemy : public ASSIMBaseCharacter
{
	GENERATED_BODY()
	
// Variables
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Components", DisplayName = "HealthComponent")
	TObjectPtr<USSIMEnemyHealthComponent> EnemyHealthComponent;
	
// Overriden Functions
public:
	ASSIMBaseEnemy();
	
};