// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMBaseUI.h"

#include "SSIM/SSIM.h"
#include "SSIM/Characters/Player/SSIMPlayer.h"

void USSIMBaseUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetReferences();
}

void USSIMBaseUI::SetReferences()
{
	if (!IsValid(GetOwningPlayerPawn()))
	{
		UE_LOG(LogSSIMUIInitialization, Error, TEXT("%s | Owning Player Pawn is not valid"), TEXT(__FUNCTION__));
	}
	SSIMPlayer = CastChecked<ASSIMPlayer>(GetOwningPlayerPawn());
}
