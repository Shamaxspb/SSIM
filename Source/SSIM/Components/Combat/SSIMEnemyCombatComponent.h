// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMBaseCombatComponent.h"
#include "SSIMEnemyCombatComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SSIM_API USSIMEnemyCombatComponent : public USSIMBaseCombatComponent
{
	GENERATED_BODY()
	
	
// Variables
#pragma region Stats
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Combat|Attack", 
			  meta = (ClampMin = 0, ClampMax = 3, UIMin = 0, UIMax = 3))
	int32 RegularAttackDamage;
	
#pragma endregion Stats	
	

// My Functions
public:
	virtual void StartAttack() override;
	
};
