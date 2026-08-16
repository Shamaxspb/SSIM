// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMDashAbility.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "SSIM/SSIM.h"
#include "SSIM/Characters/Player/SSIMPlayer.h"
#include "SSIM/Core/SSIMGameplayTags.h"

void USSIMDashAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
                                       const FGameplayAbilityActorInfo* ActorInfo,
                                       const FGameplayAbilityActivationInfo ActivationInfo, 
                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UE_LOG(LogSSIMGameplayAbilities, Warning, TEXT("%s | Dash"), TEXT(__FUNCTION__));
	
	UAbilityTask_WaitGameplayEvent* WaitTakeDamageEvent =  
		UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, TAG_Event_TakeDamage);
	WaitTakeDamageEvent->EventReceived.AddUniqueDynamic(this, &USSIMDashAbility::HandleTakeDamage);
	
}

void USSIMDashAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, 
								  const FGameplayAbilityActorInfo* ActorInfo,
								  const FGameplayAbilityActivationInfo ActivationInfo, 
								  bool bReplicateEndAbility, 
								  bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	ASSIMPlayer* Player = Cast<ASSIMPlayer>(GetAvatarActorFromActorInfo());
	
	Player->SetPlayerBrakingDecelerationWalkingToDefault();
	Player->GetCharacterMovement()->BrakingDecelerationFalling = 0.f;
	Player->SetPlayerGravityScaleToDefault();
	
	// SetDefaultDamageCollision();
	
	Player->GetCharacterMovement()->StopMovementImmediately();
	Player->GetMesh()->GetAnimInstance()->Montage_Stop(PlayerDashMontage->BlendOut.GetBlendTime());
	
	if (bShowDebug)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Dash ENDED"), TEXT(__FUNCTION__));
	}
}

void USSIMDashAbility::HandleTakeDamage(FGameplayEventData Payload)
{
	// GetWorld()->GetTimerManager().ClearTimer(WaitTakeDamageEvent);
}

void USSIMDashAbility::StartDash()
{
	if (!IsValid(GetAvatarActorFromActorInfo()))
	{
		UE_LOG(LogSSIMGameplayAbilities, Error, TEXT("%s | Avatar is not valid"), TEXT(__FUNCTION__));
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
	
	if (!IsValid(PlayerDashMontage))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | PlayerDashAnimation is not valid"), TEXT(__FUNCTION__));
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
		
	ASSIMPlayer* Player = Cast<ASSIMPlayer>(GetAvatarActorFromActorInfo());
	if (!IsValid(Player))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | Cast to SSIMPlayer failed"), TEXT(__FUNCTION__));
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
	
	Player->GetMesh()->GetAnimInstance()->Montage_Play(PlayerDashMontage);
	
	Player->StopJumping();
	//Player->LaunchCharacter(GetDashLaunchVelocity() ,true, true);
	Player->GetCharacterMovement()->GravityScale = DashGravityScale;
	
	//SetDashDamageCollision();
	
	/*GetWorld()->GetTimerManager().SetTimer(
									DashInProcessTimerHandle, 
									this, 
									&USSIMPlayerDashComponent::EndDash, 
									DashDuration, 
									false);*/
	
	if (Player->GetCharacterMovement()->IsFalling())
	{
		// Player->LandedDelegate.AddUniqueDynamic(this, &USSIMDashAbility::OnLandedFromDashHandler);
		Player->GetCharacterMovement()->BrakingDecelerationFalling = DashBrakingDecelerationFalling;
	}

}
