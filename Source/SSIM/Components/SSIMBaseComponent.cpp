// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMBaseComponent.h"

#include "GameFramework/Character.h"
#include "SSIM/SSIM.h"


USSIMBaseComponent::USSIMBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void USSIMBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	SetReferences();
}


// My Functions
void USSIMBaseComponent::SetReferences()
{
	// Characters references
	if (!IsValid(GetOwner()))
	{
		UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("%s | Owner is not valid"), TEXT(__FUNCTION__));
		return;
	}
	SSIMCharacter = CastChecked<ACharacter>(GetOwner());
	
	// Anim Instance
	if (!IsValid(SSIMCharacter->GetMesh()))
	{
		UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("%s | Mesh is not valid"), TEXT(__FUNCTION__));
		return;
	}
	if (!IsValid(SSIMCharacter->GetMesh()->GetAnimInstance()))
	{
		UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("%s | Anim Instance is not valid"), TEXT(__FUNCTION__));
		return;
	}
	AnimInstance = SSIMCharacter->GetMesh()->GetAnimInstance();
}
