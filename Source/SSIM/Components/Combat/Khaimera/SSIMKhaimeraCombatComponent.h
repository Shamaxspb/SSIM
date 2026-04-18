// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIM/Components/Combat/SSIMEnemyCombatComponent.h"
#include "SSIMKhaimeraCombatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SSIM_API USSIMKhaimeraCombatComponent : public USSIMEnemyCombatComponent
{
	GENERATED_BODY()
	
protected:
	virtual UAnimMontage* GetAttackMontage() override;
	
};
