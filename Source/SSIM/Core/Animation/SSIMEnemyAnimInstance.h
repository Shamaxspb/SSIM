// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIM/Player/Animation/SSIMAnimInstance.h"

#include "SSIMEnemyAnimInstance.generated.h"

class ASSIMBaseEnemy;


UCLASS()
class SSIM_API USSIMEnemyAnimInstance : public USSIMAnimInstance
{
	GENERATED_BODY()
	
// Variables
protected:
	UPROPERTY(BlueprintReadOnly, Category = "SSIM|References")
	TObjectPtr<ASSIMBaseEnemy> SSIMBaseEnemy;
	
	UPROPERTY(BlueprintReadOnly, Category = "SSIM|Values")
	float VelocityY;
	
// Overriden Functions
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
// My Functions
protected:
	virtual void SetOwnerReference() override;
	
	void UpdateCharacterVelocity();
};
