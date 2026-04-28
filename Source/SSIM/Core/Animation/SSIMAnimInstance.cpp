// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMAnimInstance.h"

#include "SSIM/SSIM.h"


// Overriden Functions
void USSIMAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	SetOwnerReference();

}

void USSIMAnimInstance::SetOwnerReference()
{
	if (GetWorld()->IsGameWorld()) // to prevent log spamming in editor
	{
		if (!IsValid(TryGetPawnOwner()))
		{
			UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("%s : TryGetPawnOwner is not valid"), TEXT(__FUNCTION__));
		}
		UE_LOG(LogSSIMInheritance, Warning, TEXT("%s | SetOwnerReference() is not overriden (%s)"), TEXT(__FUNCTION__), *TryGetPawnOwner()->GetName());
	}
}
