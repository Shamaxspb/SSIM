// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMUpwardAttack.h"

#include "SSIM/SSIM.h"
#include "SSIM/Characters/Player/SSIMPlayer.h"

UAnimMontage* USSIMUpwardAttack::GetAttackMontage()
{
	if (Player->GetCharacterMovement()->IsFalling())
	{
		if (AirUpwardAttackMontages.IsEmpty())
		{
			UE_LOG(LogSSIMValidations, Error, TEXT("%s | No Air Frontal Attack Montages found"), TEXT(__FUNCTION__));
			return nullptr;
		}
		AttackMontage = AirUpwardAttackMontages[FMath::RandHelper(AirUpwardAttackMontages.Num())];			
	}
	else
	{
		if (UpwardAttackMontages.IsEmpty())
		{
			UE_LOG(LogSSIMValidations, Error, TEXT("%s | No FRONTAL Attack Montages found"), TEXT(__FUNCTION__));
			return nullptr;
		}
		AttackMontage = UpwardAttackMontages[FMath::RandHelper(UpwardAttackMontages.Num())];			
	}
	
	return AttackMontage;
}
