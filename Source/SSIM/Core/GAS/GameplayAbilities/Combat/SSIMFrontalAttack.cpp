// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMFrontalAttack.h"

#include "SSIM/SSIM.h"
#include "SSIM/Characters/Player/SSIMPlayer.h"

void USSIMFrontalAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	
	
	
	
}

void USSIMFrontalAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	
	
	
	
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

UAnimMontage* USSIMFrontalAttack::GetAttackMontage()
{
	if (Player->GetCharacterMovement()->IsFalling())
	{
		if (AirAttackMontages.IsEmpty())
		{
			UE_LOG(LogSSIMValidations, Error, TEXT("%s | No Air Frontal Attack Montages found"), TEXT(__FUNCTION__));
			return nullptr;
		}
		return AirAttackMontages[FMath::RandHelper(AirAttackMontages.Num())];			
	}
	else
	{
		if (UpperBodyAttackMontages.IsEmpty())
		{
			UE_LOG(LogSSIMValidations, Error, TEXT("%s | No Frontal Attack Montages found"), TEXT(__FUNCTION__));
			return nullptr;
		}
		return UpperBodyAttackMontages[FMath::RandHelper(UpperBodyAttackMontages.Num())];			
	}
}
