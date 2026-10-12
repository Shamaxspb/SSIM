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
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | Hanging is not valid"), TEXT(__FUNCTION__));
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false);
	}
	
	Player->PlayAnimMontage(HealingMontage);
	CommitAbilityCost(Handle, ActorInfo, ActivationInfo);
	
	Player->MoveInputValue = 0.f;
	Player->GetCharacterMovement()->StopMovementImmediately();
	Player->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	Player->StopJumping();
	Player->DisableInput(Cast<APlayerController>(Player->GetController()));
	
	const FGameplayEventData Payload;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Player,TAG_Event_Heal_Started,Payload);
	
	UAbilityTask_WaitDelay* HealingTimeDelay = UAbilityTask_WaitDelay::WaitDelay(this, HealingAirTime);
	HealingTimeDelay->OnFinish.AddDynamic(this, &USSIMHealAbility::EndAirHanging);
	HealingTimeDelay->ReadyForActivation();
}

void USSIMHealAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	Player->EnableInput(Cast<APlayerController>(Player->GetController()));
	
	const FGameplayEventData Payload;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Player,TAG_Event_Heal_Ended,Payload);
}

void USSIMHealAbility::EndAirHanging()
{
	Player->StopAnimMontage(HealingMontage);
	Player->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	
	SSIMGASHelper::ApplyHealToPlayer(this, HealAmount);
	K2_EndAbility();
}
