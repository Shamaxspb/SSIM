// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SSIMBaseUI.generated.h"

class ASSIMPlayer;


UCLASS(Abstract, NotBlueprintable)
class SSIM_API USSIMBaseUI : public UUserWidget
{
	GENERATED_BODY()
	
// Variables
protected:
	UPROPERTY()
	TObjectPtr<ASSIMPlayer> SSIMPlayer;
	
private:
	virtual void SetReferences();
	
};
