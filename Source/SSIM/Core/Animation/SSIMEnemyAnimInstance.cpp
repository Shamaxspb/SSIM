// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMEnemyAnimInstance.h"

#include "SSIM/SSIM.h"
#include "SSIM/Characters/Enemies/SSIMBaseEnemy.h"

void USSIMEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	UpdateCharacterVelocity();
}

void USSIMEnemyAnimInstance::SetOwnerReference()
{
	if (TryGetPawnOwner() == nullptr)
	{
		if (GetWorld()->IsGameWorld()) // to prevent log spamming in editor
		{
			UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("%s : TryGetPawnOwner is not valid"), TEXT(__FUNCTION__));
		}
		return;
	}
	
	SSIMBaseEnemy = CastChecked<ASSIMBaseEnemy>(TryGetPawnOwner());
}

void USSIMEnemyAnimInstance::UpdateCharacterVelocity()
{
	if (!IsValid(SSIMBaseEnemy))
	{
		if (GetWorld()->IsGameWorld()) // to prevent log spamming in editor
		{
			UE_LOG(LogSSIMValidations, Warning, TEXT("%s : SSIMBaseEnemy is not valid"), TEXT(__FUNCTION__));	
		}
		return;
	}
	
	VelocityY = SSIMBaseEnemy->GetVelocity().Length();
}
