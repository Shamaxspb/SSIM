// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerAnimInstance.h"

#include "SSIM/SSIM.h"
#include "SSIM/Characters/Player/SSIMPlayer.h"

void USSIMPlayerAnimInstance::SetOwnerReference()
{
	Super::SetOwnerReference();

	if (TryGetPawnOwner() == nullptr)
	{
		if (GetWorld()->IsGameWorld()) // to prevent log spamming in editor
		{
			UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("%s : TryGetPawnOwner is not valid"), TEXT(__FUNCTION__));
		}
		return;
	}
	
	SSIMPlayer = CastChecked<ASSIMPlayer>(TryGetPawnOwner());
}
