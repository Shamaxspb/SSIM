// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMPlayerAttackAbility.h"

#include "SSIMFrontalAttack.generated.h"

/**
 * 
 */
UCLASS()
class SSIM_API USSIMFrontalAttack : public USSIMPlayerAttackAbility
{
	GENERATED_BODY()
	
// Variables
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations|FrontalAttack")
	TArray<TObjectPtr<UAnimMontage>> UpperBodyAttackMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations|FrontalAttack")
	TArray<TObjectPtr<UAnimMontage>> FullBodyAttackMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations")
	TArray<TObjectPtr<UAnimMontage>> AirAttackMontages;
	
// Custom Functions
	virtual UAnimMontage* GetAttackMontage() override;
};
