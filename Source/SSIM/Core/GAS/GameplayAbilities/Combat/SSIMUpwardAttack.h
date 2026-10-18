// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMPlayerAttackAbility.h"

#include "SSIMUpwardAttack.generated.h"

/**
 * 
 */
UCLASS()
class SSIM_API USSIMUpwardAttack : public USSIMPlayerAttackAbility
{
	GENERATED_BODY()
	
// Variables
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations|UpwardAttack")
	TArray<TObjectPtr<UAnimMontage>> UpwardAttackMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations|UpwardAttack")
	TArray<TObjectPtr<UAnimMontage>> AirUpwardAttackMontages;

// Custom Functions
	virtual UAnimMontage* GetAttackMontage() override;
};
