// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMFrontalAttack.h"

#include "SSIM/SSIM.h"
#include "SSIM/Characters/Player/SSIMPlayer.h"

UAnimMontage* USSIMFrontalAttack::GetAttackMontage()
{
	if (Player->GetCharacterMovement()->IsFalling())
	{
		if (AirAttackMontages.IsEmpty())
		{
			UE_LOG(LogSSIMValidations, Error, TEXT("%s | No Air Frontal Attack Montages found"), TEXT(__FUNCTION__));
			return nullptr;
		}
		AttackMontage = AirAttackMontages[FMath::RandHelper(AirAttackMontages.Num())];			
	}
	else
	{
		if (UpperBodyAttackMontages.IsEmpty())
		{
			UE_LOG(LogSSIMValidations, Error, TEXT("%s | No FRONTAL Attack Montages found"), TEXT(__FUNCTION__));
			return nullptr;
		}
		AttackMontage = UpperBodyAttackMontages[FMath::RandHelper(UpperBodyAttackMontages.Num())];			
	}
	return AttackMontage;
}
