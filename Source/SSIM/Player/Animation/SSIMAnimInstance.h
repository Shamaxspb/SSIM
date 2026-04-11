// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "SSIMAnimInstance.generated.h"


UCLASS(Abstract)
class SSIM_API USSIMAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
// Overriden Functions
public:
	virtual void NativeInitializeAnimation() override;
	
// My Functions
protected:
	virtual void SetOwnerReference();
	
};
