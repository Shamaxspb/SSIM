// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMHealAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "SSIM/Core/Helpers/SSIMGASHelper.h"
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
	CommitAbilityCost(Handle, ActorInfo, ActivationInfo);
	
	Player->GetCharacterMovement()->StopMovementImmediately();
	Player->StopJumping();
	Player->GetCharacterMovement()->GravityScale = 0.0f;
	
	FGameplayEventData Payload;
	Payload.EventTag = TAG_Event_Heal_Started;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Player,TAG_Event_Heal_Started,Payload);
	
	UAbilityTask_WaitDelay* HealingTimeDelay = UAbilityTask_WaitDelay::WaitDelay(this, HealingAirTime);
	HealingTimeDelay->OnFinish.AddDynamic(this, &USSIMHealAbility::EndAirHanging);
	HealingTimeDelay->ReadyForActivation();
	
	/*FTimerHandle AirHangingTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		AirHangingTimerHandle,
		this, &USSIMHealAbility::EndAirHanging,
		HealingAirTime,
		false
		);*/
}

void USSIMHealAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
}

void USSIMHealAbility::EndAirHanging()
{
	Player->StopAnimMontage(HealingMontage);
	Player->GetCharacterMovement()->GravityScale = 7.0f;
	
	SSIMGASHelper::ApplyHealToPlayer(this, HealAmount);
	K2_EndAbility();
}
