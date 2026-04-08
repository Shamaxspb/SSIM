// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMBasePlayerComponent.h"

#include "SSIM/SSIM.h"
#include "SSIM/Player/SSIMPlayer.h"


// Overriden Functions
USSIMBasePlayerComponent::USSIMBasePlayerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void USSIMBasePlayerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SetReferences();
	
}


void USSIMBasePlayerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}



// My Functions
void USSIMBasePlayerComponent::SetReferences()
{
	// SSIMPlayer
	if (!IsValid(GetOwner()))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s: GetOwner is not valid"), TEXT(__FUNCTION__));
		return;
	}
	SSIMPlayer = Cast<ASSIMPlayer>(GetOwner());
	
	// SSIM Anim Instance
	if (!IsValid(SSIMPlayer->GetMesh()))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s: Owner mesh is not valid"), TEXT(__FUNCTION__));
		return;
	}
	if (!IsValid(SSIMPlayer->GetMesh()->GetAnimInstance()))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s: Anim Instance is not valid"), TEXT(__FUNCTION__));
		return;
	}
	SSIMAnimInstance = SSIMPlayer->GetMesh()->GetAnimInstance();
	
}

