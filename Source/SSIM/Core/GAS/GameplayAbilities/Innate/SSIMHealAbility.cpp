// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMHealAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "SSIM/SSIM.h"
#include "SSIM/Core/SSIMGameplayTags.h"
#include "SSIM/Characters/Player/SSIMPlayer.h"

void USSIMHealAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!IsValid(HealingMontage))
	{
		if (bShowDebug)
		{
			UE_LOG(LogSSIMValidations, Error, TEXT("%s | Hanging is not valid"), TEXT(__FUNCTION__));
		}
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false);
	}
	
	Player->PlayAnimMontage(HealingMontage);
	
	FGameplayEventData Payload;
	Payload.EventTag = TAG_Event_Heal_Started;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Player,TAG_Event_Heal_Started,Payload);
}

void USSIMHealAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
