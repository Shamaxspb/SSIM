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
	// Function is empty intentional, it must be overriden in child classes
	// Function is not pure virtual, because UCLASS functions cannot be such
}
