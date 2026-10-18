// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPogoAttack.h"

#include "SSIM/SSIM.h"

UAnimMontage* USSIMPogoAttack::GetAttackMontage()
{
	if (PogoMontages.IsEmpty())
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | No Air Frontal Attack Montages found"), TEXT(__FUNCTION__));
		return nullptr;
	}
	AttackMontage = PogoMontages[FMath::RandHelper(PogoMontages.Num())];
	
	return AttackMontage;
}
