// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "SSIMPlayerInputComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(PlayerComponents))
class SSIM_API USSIMPlayerInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

	UInputAction* MoveInputAction;
	
public:
	USSIMPlayerInputComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	virtual void BeginPlay() override;

};
