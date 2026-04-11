// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIM/Player/Animation/SSIMAnimInstance.h"

#include "SSIMPlayerAnimInstance.generated.h"

class ASSIMPlayer;


UCLASS()
class SSIM_API USSIMPlayerAnimInstance : public USSIMAnimInstance
{
	GENERATED_BODY()
	
// Variables
protected:
	UPROPERTY(BlueprintReadOnly, Category = "SSIM|References")
	TObjectPtr<ASSIMPlayer> SSIMPlayer;
	
// Overriden Functions
public:

	
// My Functions
protected:
	virtual void SetOwnerReference() override;
};
