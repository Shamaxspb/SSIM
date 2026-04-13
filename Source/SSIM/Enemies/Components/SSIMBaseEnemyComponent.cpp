// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMBaseEnemyComponent.h"

#include "SSIM/SSIM.h"
#include "SSIM/Enemies/Characters/SSIMBaseEnemy.h"


USSIMBaseEnemyComponent::USSIMBaseEnemyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void USSIMBaseEnemyComponent::BeginPlay()
{
	Super::BeginPlay();

	SetReferences();
}


// My Functions
void USSIMBaseEnemyComponent::SetReferences()
{
	// SSIMBaseEnemy
	if (!IsValid(GetOwner()))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | GetOwner is not valid"), TEXT(__FUNCTION__));
		return;
	}
	SSIMBaseEnemy = Cast<ASSIMBaseEnemy>(GetOwner());
	
	// SSIM Anim Instance
	if (!IsValid(SSIMBaseEnemy->GetMesh()))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | Owner mesh is not valid"), TEXT(__FUNCTION__));
		return;
	}
	if (!IsValid(SSIMBaseEnemy->GetMesh()->GetAnimInstance()))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | Anim Instance is not valid"), TEXT(__FUNCTION__));
		return;
	}
	SSIMAnimInstance = SSIMBaseEnemy->GetMesh()->GetAnimInstance();
}

