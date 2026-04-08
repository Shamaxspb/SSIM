// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMAnimInstance.h"
#include "SSIM/Player/SSIMPlayer.h"
#include "../../SSIM.h"

// Overriden Functions
void USSIMAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (TryGetPawnOwner() == nullptr)
	{
		UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("%s : TryGetPawnOwner is not valid"), TEXT(__FUNCTION__));
		return;
	}
	SSIMPlayer = CastChecked<ASSIMPlayer>(TryGetPawnOwner());
}
