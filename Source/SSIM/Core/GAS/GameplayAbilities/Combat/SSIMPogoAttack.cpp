// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPogoAttack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "SSIM/SSIM.h"
#include "SSIM/Characters/Player/SSIMPlayer.h"
#include "SSIM/Core/SSIMGameplayTags.h"

void USSIMPogoAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	
	FOnMontageEnded OnPogoAnimationEnded;
	OnPogoAnimationEnded.BindUObject(this, &USSIMPogoAttack::PogoAnimationEndedCallback);
	Player->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnPogoAnimationEnded);
		
	const FGameplayEventData PogoAnimationStartedPayload;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Player, TAG_Event_Pogo_Animation_Started, PogoAnimationStartedPayload);
}

void USSIMPogoAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	
	
	
	
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

UAnimMontage* USSIMPogoAttack::GetAttackMontage()
{
	if (PogoMontages.IsEmpty())
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | No Pogo Attack Montages found"), TEXT(__FUNCTION__));
		return nullptr;
	}
	return PogoMontages[FMath::RandHelper(PogoMontages.Num())];
}

void USSIMPogoAttack::PogoAnimationEndedCallback(UAnimMontage* PogoMontage, bool Interrupted) const
{
	const FGameplayEventData PogoAnimationEndedPayload;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Player, TAG_Event_Pogo_Animation_Ended, PogoAnimationEndedPayload);
}
