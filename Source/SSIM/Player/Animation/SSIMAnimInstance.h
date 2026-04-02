// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "SSIMAnimInstance.generated.h"

class ASSIMPlayer;

UCLASS()
class SSIM_API USSIMAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
// Variables
protected:
	UPROPERTY(BlueprintReadOnly, Category = "SSIM|References")
	ASSIMPlayer* SSIMPlayer;
	
// Overriden Functions
public:
	virtual void NativeInitializeAnimation() override;
	
};
