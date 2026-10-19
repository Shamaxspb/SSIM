// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerAttackAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "SSIM/SSIM.h"
#include "SSIM/Characters/Player/SSIMPlayer.h"
#include "SSIM/Core/SSIMGameplayTags.h"

void USSIMPlayerAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                               const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UAnimMontage* AttackMontage = GetAttackMontage();
	if (!IsValid(AttackMontage))
	{
		K2_CancelAbility();
	}
	
	Player->PlayAnimMontage(AttackMontage);
	FOnMontageEnded OnAttackAnimationEnded;
	OnAttackAnimationEnded.BindUObject(this, &USSIMPlayerAttackAbility::HandleEndMontage);
	Player->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnAttackAnimationEnded, AttackMontage);
	
	const FGameplayEventData AttackStartedPayload;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Player,TAG_Event_Attack_Started,AttackStartedPayload);
	
	if (bShowDebug)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Attack Started"), TEXT(__FUNCTION__));
	}
	
	WaitTakeDamageTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		TAG_Event_DamageTaken,
		nullptr,
		false,
		true);
	if (WaitTakeDamageTask)
	{
		WaitTakeDamageTask->EventReceived.AddDynamic(this, &USSIMPlayerAttackAbility::HandleTakeDamage);
		WaitTakeDamageTask->ReadyForActivation();
	}
}

void USSIMPlayerAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	
	if (!bWasCancelled)
	{
		const FGameplayEventData Payload;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Player, TAG_Event_Attack_Ended, Payload);
	}
	
	if (bShowDebug)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Attack Ended"), TEXT(__FUNCTION__));
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void USSIMPlayerAttackAbility::HandleTakeDamage(FGameplayEventData Payload)
{
	FGameplayEventData AttackInterruptedPayload;
	AttackInterruptedPayload.EventTag = TAG_Event_Attack_Interrupted.GetTag();
	AttackInterruptedPayload.Instigator = Payload.Instigator;
	AttackInterruptedPayload.Target = GetOwningActorFromActorInfo();
	GetAbilitySystemComponentFromActorInfo()->HandleGameplayEvent(TAG_Event_Heal_Interrupted, &AttackInterruptedPayload);
	
	if (bShowDebug)
	{
		UE_LOG(LogSSIMStatsCalculation, Log, TEXT("%s | Attack interrupted by taking damage"), TEXT(__FUNCTION__));
	}
	K2_CancelAbility();
}

UAnimMontage* USSIMPlayerAttackAbility::GetAttackMontage()
{
	UE_LOG(LogSSIMInheritance, Warning, TEXT("%s | GetAttackMontage() is not overriden"), *GetAvatarActorFromActorInfo()->GetName());
	return nullptr;
}

void USSIMPlayerAttackAbility::HandleEndMontage(UAnimMontage* PogoMontage, bool Interrupted)
{
	K2_EndAbility();
}
