// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SSIMBasePlayerComponent.generated.h"

class ASSIMPlayer;

UCLASS(Abstract)
class SSIM_API USSIMBasePlayerComponent : public UActorComponent
{
	GENERATED_BODY()

// Variables
protected:
	UPROPERTY(BlueprintReadOnly, Category = "SSIM|References")
	ASSIMPlayer* SSIMPlayer;

	
// Overriden Functions
public:
	USSIMBasePlayerComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
