// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMKhaimeraCombatComponent.h"

#include "SSIM/SSIM.h"

UAnimMontage* USSIMKhaimeraCombatComponent::GetAttackMontage()
{
	if (!IsValid(AttackMontage))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | Attack Montage is not valid"), TEXT(__FUNCTION__));
		return nullptr;
	}
	return AttackMontage;
}
