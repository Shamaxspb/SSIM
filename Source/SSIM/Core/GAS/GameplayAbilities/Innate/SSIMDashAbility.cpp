// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMDashAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "SSIM/SSIM.h"
#include "SSIM/Characters/Player/SSIMPlayer.h"
#include "SSIM/Core/SSIMGameplayTags.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimNotifies/AnimNotify.h"

void USSIMDashAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
                                       const FGameplayAbilityActorInfo* ActorInfo,
                                       const FGameplayAbilityActivationInfo ActivationInfo, 
                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!IsValid(DashMontage))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | DashMontage is not valid"), TEXT(__FUNCTION__));
		K2_CancelAbility();
		return;
	}
	
	UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
	if (!IsValid(AnimInstance))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | AnimInstance is not valid"), TEXT(__FUNCTION__));
		K2_CancelAbility();
	}
	
	BoundAnimInstance = AnimInstance;
	AnimInstance->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &USSIMDashAbility::HandleSwitchMovementModeMontageNotify);
	
	Player->PlayAnimMontage(DashMontage);
	
	if (FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(DashMontage))
	{
		ActiveMontageInstanceID = MontageInstance->GetInstanceID();
	}
	
	if (Player->GetCharacterMovement()->IsFalling())
	{
		bStartedInAir = true;
		
		Player->GetCharacterMovement()->BrakingDecelerationFalling = DashBrakingDecelerationFalling; // Would it work in MovementMode::Flying?
		
		Player->LandedDelegate.AddUniqueDynamic(this, &USSIMDashAbility::EndDashByLanding);
	}
	else
	{
		bStartedInAir = false;
		
		Player->GetCharacterMovement()->BrakingDecelerationFalling = DashBrakingDecelerationWalking;
		
		WaitDashEnd = UAbilityTask_WaitDelay::WaitDelay(this, DashDuration);
		WaitDashEnd->OnFinish.AddUniqueDynamic(this, &USSIMDashAbility::EndDashByDuration);
		WaitDashEnd->ReadyForActivation();
	}
	
	Player->MoveInputValue = 0.f;
	Player->GetCharacterMovement()->StopMovementImmediately();
	Player->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	Player->StopJumping();
	//Player->DisableInput(Cast<APlayerController>(Player->GetController()));
	
	// LaunchCharacter or Root Motion?
	//Player->LaunchCharacter(GetDashLaunchVelocity() ,true, true);
	
	const FGameplayEventData Payload;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Player,TAG_Event_Dash_Started,Payload);
	
	GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(TAG_State_Modifier_MovementBlock.GetTag());
	
	WaitTakeDamageTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, TAG_Event_DamageTaken);
	if (WaitTakeDamageTask)
	{
		WaitTakeDamageTask->EventReceived.AddUniqueDynamic(this, &USSIMDashAbility::EndDashByTakingDamage);
		WaitTakeDamageTask->ReadyForActivation();
	}
	
	WaitPogoStartedTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, TAG_Event_Pogo_Started);
	if (WaitPogoStartedTask)
	{
		WaitPogoStartedTask->EventReceived.AddUniqueDynamic(this, &USSIMDashAbility::EndDashByPogo);
		WaitPogoStartedTask->ReadyForActivation();
	}
	
	if (bShowDebug)
	{
		UE_LOG(LogSSIMGameplayAbilities, Log, TEXT("%s | Dash started %s"), TEXT(__FUNCTION__),
			bStartedInAir ? TEXT("in air") : TEXT("on the ground"));
	}
}

void USSIMDashAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, 
								  const FGameplayAbilityActorInfo* ActorInfo,
								  const FGameplayAbilityActivationInfo ActivationInfo, 
								  bool bReplicateEndAbility, 
								  bool bWasCancelled)
{
	// I am binding here to MontageNotify "SwitchMovementMode"
	// Looks like create proper AnimNotify is easier and more flexible than this approach in C++
	if (UAnimInstance* AnimInstance = BoundAnimInstance.Get())
	{
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &ThisClass::HandleSwitchMovementModeMontageNotify);
	}
	BoundAnimInstance.Reset();
	ActiveMontageInstanceID = INDEX_NONE;
	//
	
	if (bStartedInAir)
	{
		Player->LandedDelegate.RemoveDynamic(this, &USSIMDashAbility::EndDashByLanding);
		Player->GetCharacterMovement()->BrakingDecelerationFalling = 0.f;
	}
	else
	{
		Player->SetPlayerBrakingDecelerationWalkingToDefault();
	}
	
	//Player->GetCharacterMovement()->StopMovementImmediately();
	Player->StopAnimMontage(DashMontage);
	SetCollisionParametersToDefault();
	GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(TAG_State_Modifier_MovementBlock.GetTag());
	
	const FGameplayEventData Payload;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Player,TAG_Event_Dash_Ended,Payload);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void USSIMDashAbility::SetCollisionParametersToDash() const
{
	Player->GetHitRegistrationCollision()->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	Player->GetContactDamageCollision()->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	
	Player->GetHitRegistrationCollision()->SetCapsuleHalfHeight(DashHitRegistrationCollisionHalfHeight);
	Player->GetHitRegistrationCollision()->SetCapsuleRadius(DashHitRegistrationCollisionRadius);
	
	Player->GetContactDamageCollision()->SetCapsuleHalfHeight(DashContactDamageCollisionHalfHeight);
	Player->GetContactDamageCollision()->SetCapsuleRadius(DashContactDamageCollisionRadius);
}

void USSIMDashAbility::SetCollisionParametersToDefault() const
{
	Player->GetContactDamageCollision()->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	Player->GetHitRegistrationCollision()->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	
	Player->GetHitRegistrationCollision()->SetCapsuleHalfHeight(Player->DefaultHitRegistrationCollisionHalfHeight);
	Player->GetHitRegistrationCollision()->SetCapsuleRadius(Player->DefaultHitRegistrationCollisionRadius);
	
	Player->GetContactDamageCollision()->SetCapsuleHalfHeight(Player->DefaultContactDamageCollisionHalfHeight);
	Player->GetContactDamageCollision()->SetCapsuleRadius(Player->DefaultContactDamageCollisionRadius);
}

void USSIMDashAbility::HandleSwitchMovementModeMontageNotify(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	if (NotifyName != SwitchMovementModeNotifyName)
	{
		return;
	}
	
	// Ignore notifications from another montage.
	if (Payload.SequenceAsset != DashMontage)
	{
		return;
	}
	
	// Ignore another playback instance of the same montage.
	if (ActiveMontageInstanceID != INDEX_NONE &&
		Payload.MontageInstanceID != ActiveMontageInstanceID)
	{
		return;
	}
	
	GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(TAG_State_Modifier_MovementBlock.GetTag());
	
	if (bStartedInAir)
	{
		Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	}
	else
	{
		Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

void USSIMDashAbility::EndDashByTakingDamage(FGameplayEventData Payload)
{
	if (bShowDebug)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Dash Ability Cancelled by taking damage"), TEXT(__FUNCTION__));
	}
	K2_CancelAbility();
}

void USSIMDashAbility::EndDashByPogo(FGameplayEventData Payload)
{
	if (bShowDebug)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Dash Ability Ended by Pogo"), TEXT(__FUNCTION__));
	}
	K2_EndAbility();
}

void USSIMDashAbility::EndDashByLanding(const FHitResult& Hit)
{
	if (bShowDebug)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Dash Ability Ended by landing"), TEXT(__FUNCTION__));
	}
	K2_EndAbility();
}

void USSIMDashAbility::EndDashByDuration()
{
	if (bShowDebug)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Dash Ability Ended by duration"), TEXT(__FUNCTION__));
	}
	K2_EndAbility();
}
