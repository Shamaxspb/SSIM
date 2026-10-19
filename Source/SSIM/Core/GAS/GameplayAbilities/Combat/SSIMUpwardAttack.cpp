// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMUpwardAttack.h"

#include "SSIM/SSIM.h"
#include "SSIM/Characters/Player/SSIMPlayer.h"

void USSIMUpwardAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	
	
	
	
}

void USSIMUpwardAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	
	
	
	
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


UAnimMontage* USSIMUpwardAttack::GetAttackMontage()
{
	if (Player->GetCharacterMovement()->IsFalling())
	{
		if (AirUpwardAttackMontages.IsEmpty())
		{
			UE_LOG(LogSSIMValidations, Error, TEXT("%s | No Air Upward Attack Montages found"), TEXT(__FUNCTION__));
			return nullptr;
		}
		return AirUpwardAttackMontages[FMath::RandHelper(AirUpwardAttackMontages.Num())];			
	}
	else
	{
		if (UpwardAttackMontages.IsEmpty())
		{
			UE_LOG(LogSSIMValidations, Error, TEXT("%s | No Upward Attack Montages found"), TEXT(__FUNCTION__));
			return nullptr;
		}
		return UpwardAttackMontages[FMath::RandHelper(UpwardAttackMontages.Num())];			
	}
}
