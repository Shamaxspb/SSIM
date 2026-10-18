// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMPlayerAttackAbility.h"

#include "SSIMPogoAttack.generated.h"

/**
 * 
 */
UCLASS()
class SSIM_API USSIMPogoAttack : public USSIMPlayerAttackAbility
{
	GENERATED_BODY()
	
// Variables
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations|PogoAttack")
	TArray<TObjectPtr<UAnimMontage>> PogoMontages;	
	
	
// Custom Functions
	virtual UAnimMontage* GetAttackMontage() override;
};
