// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMDashAbility.h"

#include "SSIM/SSIM.h"

void USSIMDashAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
                                   const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, 
                                   const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UE_LOG(LogSSIMGameplayAbilities, Log, TEXT("%s | Dash"), TEXT(__FUNCTION__));
}
