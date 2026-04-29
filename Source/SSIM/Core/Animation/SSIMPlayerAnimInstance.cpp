// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerAnimInstance.h"

#include "Kismet/KismetMathLibrary.h"
#include "SSIM/SSIM.h"
#include "SSIM/Characters/Player/SSIMPlayer.h"
#include "SSIM/Components/Combat/SSIMPlayerCombatComponent.h"
#include "SSIM/Components/Stats/SSIMPlayerStatsComponent.h"

void USSIMPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	//SSIMPlayer->GetPlayerCombatComponent()->OnPogoAnimationStartedDelegate.AddDynamic(this, &USSIMPlayerAnimInstance::OnPogoAnimationStartedHandler);
	SSIMPlayer->GetPlayerCombatComponent()->OnAttackStartedDelegate.AddDynamic(this, &USSIMPlayerAnimInstance::OnPogoAnimationStartedHandler);
	// SSIMPlayer->GetPlayerCombatComponent()->OnPogoEndedDelegate.AddDynamic(this, &USSIMPlayerAnimInstance::OnPogoEndedHandler);
	SSIMPlayer->GetPlayerCombatComponent()->OnPogoAnimationEndedDelegate.AddDynamic(this, &USSIMPlayerAnimInstance::OnPogoAnimationEndedHandler);
	
	SSIMPlayer->GetPlayerStatsComponent()->OnDamageReceivedDelegate.AddDynamic(this, &USSIMPlayerAnimInstance::OnDamageReceivedHandler);
}

void USSIMPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (PogoBonesState == EPogoBonesState::EPBS_BlendingIn)
	{
		BlendElapsedTime += DeltaSeconds;
		
		float Alpha			= FMath::Clamp(BlendElapsedTime / PogoBlendInDuration, 0.f, 1.f);
		float SmoothedAlpha = FMath::InterpEaseInOut(0.f, 1.f, Alpha, 2.f);
		
		// This is better be done via foreach, TArray stores values, not rotator references, need to figure it out 
		BlendInPogoBone(SmoothedAlpha, PogoCurrentAdditionalRotation_Pelvis,   PogoModifiedAdditionalRotation_Pelvis);
		BlendInPogoBone(SmoothedAlpha, PogoCurrentAdditionalRotation_Spine_01, PogoModifiedAdditionalRotation_Spine_01);
		BlendInPogoBone(SmoothedAlpha, PogoCurrentAdditionalRotation_Spine_02, PogoModifiedAdditionalRotation_Spine_02);
		BlendInPogoBone(SmoothedAlpha, PogoCurrentAdditionalRotation_Thigh_L,  PogoModifiedAdditionalRotation_Thigh_L);
		BlendInPogoBone(SmoothedAlpha, PogoCurrentAdditionalRotation_Thigh_R,  PogoModifiedAdditionalRotation_Thigh_R);
		
		if (SmoothedAlpha >= 1.f)
		{
			PogoBonesState = EPogoBonesState::EPBS_ModifiedRotation;
			BlendElapsedTime = 0.f;
			PogoBlendInDuration = DefaultPogoBlendInDuration;
			
			if (bShowPogoBlendLogs)
			{
				UE_LOG(LogSSIMAnimation, Warning, TEXT("%s | Blend In COMPLETED, State: %s"), TEXT(__FUNCTION__), *UEnum::GetValueAsString(PogoBonesState));
			}
		}
	}
	
	if (PogoBonesState == EPogoBonesState::EPBS_BlendingOut)
	{
		BlendElapsedTime += DeltaSeconds;
		
		float Alpha			= FMath::Clamp(BlendElapsedTime / PogoBlendOutDuration, 0.f, 1.f);
		float SmoothedAlpha = FMath::InterpEaseInOut(0.f, 1.f, Alpha, 2.f);
		
		// This is better be done via foreach, TArray stores values, not rotator references, need to figure it out 
		BlendOutPogoBone(SmoothedAlpha, PogoCurrentAdditionalRotation_Pelvis,   PogoModifiedAdditionalRotation_Pelvis);
		BlendOutPogoBone(SmoothedAlpha, PogoCurrentAdditionalRotation_Spine_01, PogoModifiedAdditionalRotation_Spine_01);
		BlendOutPogoBone(SmoothedAlpha, PogoCurrentAdditionalRotation_Spine_02, PogoModifiedAdditionalRotation_Spine_02);
		BlendOutPogoBone(SmoothedAlpha, PogoCurrentAdditionalRotation_Thigh_L,  PogoModifiedAdditionalRotation_Thigh_L);
		BlendOutPogoBone(SmoothedAlpha, PogoCurrentAdditionalRotation_Thigh_R,  PogoModifiedAdditionalRotation_Thigh_R);
		
		if (SmoothedAlpha >= 1.f)
		{
			PogoBonesState = EPogoBonesState::EPBS_DefaultRotation;
			BlendElapsedTime = 0.f;
			PogoBlendOutDuration = DefaultPogoBlendOutDuration;
			
			if (bShowPogoBlendLogs)
			{
				UE_LOG(LogSSIMAnimation, Warning, TEXT("%s | Blend Out COMPLETED, State: %s"), TEXT(__FUNCTION__), *UEnum::GetValueAsString(PogoBonesState));
			}
		}
	}
}

void USSIMPlayerAnimInstance::SetOwnerReference()
{
	if (!IsValid(TryGetPawnOwner()))
	{
		if (GetWorld()->IsGameWorld()) // to prevent log spamming in editor
		{
			UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("%s : TryGetPawnOwner is not valid"), TEXT(__FUNCTION__));
		}
		return;
	}
	
	SSIMPlayer = CastChecked<ASSIMPlayer>(TryGetPawnOwner());
}

void USSIMPlayerAnimInstance::StartBlendInPogoBones(float InBlendInDuration)

{
	if (PogoBonesState == EPogoBonesState::EPBS_ModifiedRotation || PogoBonesState == EPogoBonesState::EPBS_BlendingIn)
	{
		if (bShowPogoBlendLogs)
		{
			if (PogoBonesState != EPogoBonesState::EPBS_ModifiedRotation)
			{
				UE_LOG(LogSSIMAnimation, Warning, TEXT( "%s | Blend In Start CANCELED, Player Pogo Bones are already in (%s) state"), 
					TEXT(__FUNCTION__), 
					*UEnum::GetValueAsString(PogoBonesState));
			}
		}
		return;
	}
	if (bShowPogoBlendLogs)
	{
		UE_LOG(LogSSIMAnimation, Warning, TEXT("%s | Prepare to Start Blend In, State: %s"), 
			TEXT(__FUNCTION__), 
			*UEnum::GetValueAsString(PogoBonesState));
	}
	
	// Set values to start interpolation on Tick
	PogoBonesState = EPogoBonesState::EPBS_BlendingIn;
	PogoBlendInDuration = InBlendInDuration;
	BlendElapsedTime = 0.f;

	if (bShowPogoBlendLogs)
	{
		UE_LOG(LogSSIMAnimation, Warning, TEXT("%s | Start Blend In, State: %s"), TEXT(__FUNCTION__), *UEnum::GetValueAsString(PogoBonesState));
	}
}

void USSIMPlayerAnimInstance::StartBlendOutPogoBones(float InBlendOutDuration)
{
	if (PogoBonesState == EPogoBonesState::EPBS_DefaultRotation || PogoBonesState == EPogoBonesState::EPBS_BlendingOut)
	{
		if (bShowPogoBlendLogs)
		{
			UE_LOG(LogSSIMAnimation, Warning, TEXT( "%s | Blend Out Start CANCELED: Player Pogo Bones are already in (%s) state"), 
				TEXT(__FUNCTION__), 
				*UEnum::GetValueAsString(PogoBonesState));
		}
		return;
	}
	
	if (bShowPogoBlendLogs)
	{
		UE_LOG(LogSSIMAnimation, Warning, TEXT("%s | Prepare to Start Blend Out, State: %s"), 
			TEXT(__FUNCTION__), 
			*UEnum::GetValueAsString(PogoBonesState));
	}
	
	// Set values to start interpolation on Tick
	PogoBonesState = EPogoBonesState::EPBS_BlendingOut;
	PogoBlendOutDuration = InBlendOutDuration;
	BlendElapsedTime = 0.f;
	
	if (bShowPogoBlendLogs)
	{
		UE_LOG(LogSSIMAnimation, Warning, TEXT("%s | Start Blend Out, State: %s"), 
			TEXT(__FUNCTION__), 
			*UEnum::GetValueAsString(PogoBonesState));
	}
}

void USSIMPlayerAnimInstance::BlendInPogoBone(float InSmoothedAlpha, FRotator& InPogoBoneRotation, const FRotator InModifiedRotation)
{
	InPogoBoneRotation = UKismetMathLibrary::RLerp(
						FRotator::ZeroRotator,
						InModifiedRotation,
						   InSmoothedAlpha,
			   true);
	
}

void USSIMPlayerAnimInstance::BlendOutPogoBone(float InSmoothedAlpha, FRotator& InPogoBoneRotation, const FRotator InModifiedRotation)
{
	InPogoBoneRotation = UKismetMathLibrary::RLerp(
						InModifiedRotation,
						FRotator::ZeroRotator,
						   InSmoothedAlpha,
			   true);
	
}

void USSIMPlayerAnimInstance::OnPogoAnimationStartedHandler()
{
	if (bShowPogoBlendLogs)
	{
		UE_LOG(LogSSIMAnimation, Error, TEXT("%s"), TEXT(__FUNCTION__));
	}
	
	if (SSIMPlayer->GetPlayerCombatComponent()->PlayerAttackDirectionType == EPlayerAttackDirectionType::EPADT_Downward)
	{
		SSIMPlayer->LandedDelegate.AddUniqueDynamic(this, &USSIMPlayerAnimInstance::OnPlayerLanded);
		StartBlendInPogoBones(DefaultPogoBlendInDuration);
	}
}

void USSIMPlayerAnimInstance::OnPogoEndedHandler()
{
	if (bShowPogoBlendLogs)
	{
		UE_LOG(LogSSIMAnimation, Error, TEXT("%s"), TEXT(__FUNCTION__));
	}
	
	StartBlendOutPogoBones(DefaultPogoBlendOutDuration);
}

void USSIMPlayerAnimInstance::OnDamageReceivedHandler(const FDamageData& InDamageData)
{
	if (bShowPogoBlendLogs)
	{
		UE_LOG(LogSSIMAnimation, Error, TEXT("%s"), TEXT(__FUNCTION__));
	}
	
	StartBlendOutPogoBones(OnDamageReceivedPogoBlendOutDuration);
}

void USSIMPlayerAnimInstance::OnPogoAnimationEndedHandler(bool bInterrupted)
{
	if (bShowPogoBlendLogs)
	{
		UE_LOG(LogSSIMAnimation, Error, TEXT("%s"), TEXT(__FUNCTION__));
	}
	
	if (bInterrupted)
	{
		if (bShowPogoBlendLogs)
		{
			UE_LOG(LogSSIMAnimation, Log, TEXT("%s | Attack Ended because it was interrupted, no need to Blend Out"), 
				TEXT(__FUNCTION__));
		}
		return;
	}
	
	StartBlendOutPogoBones(DefaultPogoBlendOutDuration);
}

void USSIMPlayerAnimInstance::OnPlayerLanded(const FHitResult& Hit)
{
	if (bShowPogoBlendLogs)
	{
		UE_LOG(LogSSIMAnimation, Error, TEXT("%s"), TEXT(__FUNCTION__));
	}
	
	StartBlendOutPogoBones(OnLandedPogoBlendInDuration);
	SSIMPlayer->LandedDelegate.RemoveDynamic(this, &USSIMPlayerAnimInstance::OnPlayerLanded);
}
