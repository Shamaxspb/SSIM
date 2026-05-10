// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerAnimInstance.h"

#include "Kismet/KismetMathLibrary.h"
#include "SSIM/SSIM.h"
#include "SSIM/Characters/Player/SSIMPlayer.h"
#include "SSIM/Components/Combat/SSIMPlayerCombatComponent.h"
#include "SSIM/Components/PlayerComponents/SSIMPlayerDashComponent.h"
#include "SSIM/Components/Stats/SSIMPlayerStatsComponent.h"

void USSIMPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	SSIMPlayer->GetPlayerCombatComponent()->OnHitRegistrationDelegate.AddDynamic(this, &USSIMPlayerAnimInstance::OnHitRegistrationHandle);
	SSIMPlayer->GetPlayerCombatComponent()->OnPogoAnimationStartedDelegate.AddDynamic(this, &USSIMPlayerAnimInstance::OnPogoAnimationStartedHandler);
	SSIMPlayer->GetPlayerCombatComponent()->OnPogoAnimationEndedDelegate.AddDynamic(this, &USSIMPlayerAnimInstance::OnPogoAnimationEndedHandler);
	
	SSIMPlayer->GetPlayerDashComponent()->OnDashStartedDelegate.AddDynamic(this, &USSIMPlayerAnimInstance::OnDashStartedHandler);
	
	SSIMPlayer->GetPlayerStatsComponent()->OnDamageReceivedDelegate.AddDynamic(this, &USSIMPlayerAnimInstance::OnDamageReceivedHandler);
	
	SSIMPlayer->GetPlayerStatsComponent()->OnHealingStartedDelegate.AddDynamic(this, &USSIMPlayerAnimInstance::OnHealingStartedHandler);
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

void USSIMPlayerAnimInstance::SwapHitMontage(EPlayerAttackDirectionType InPlayerAttackDirectionType)
{
	UAnimMontage* CurrentMontage = GetCurrentActiveMontage();
	if (!IsValid(CurrentMontage))
	{
		UE_LOG(LogSSIMAnimation, Warning, TEXT("%s | Current Active Montage is not valid"), TEXT(__FUNCTION__));
		return;
	}
		
	if (bShowSwapHitMontageLogs)
	{
		UE_LOG(LogSSIMAnimation, Log, TEXT("%s | CURRENT Active Montage: %s"), TEXT(__FUNCTION__), *CurrentMontage->GetName());
	}
	
	// [0] for now, rework later
	bool bCorrectMontage = SSIMPlayer->GetPlayerCombatComponent()->UpperBodyPlayerFrontalAttackMontages.Contains(CurrentMontage);
	bool bFalling = SSIMPlayer->GetCharacterMovement()->IsFalling();
	bool bKnockbackActive = SSIMPlayer->GetIsPlayerAttackKnockbackActive();
	
	if (InPlayerAttackDirectionType == EPlayerAttackDirectionType::EPADT_Frontal 
		&& 
		bCorrectMontage
		&&
		!bFalling
		&&
		!bKnockbackActive)
	{
		
		UAnimMontage* CurrentUpperBodyAttackMontage = SSIMPlayer->GetPlayerCombatComponent()->UpperBodyPlayerFrontalAttackMontages[0];
		UAnimMontage* FullBodyAttackMontage = SSIMPlayer->GetPlayerCombatComponent()->FullBodyPlayerFrontalAttackMontages[0];
		
		float CurrentAttackMontagePosition = Montage_GetPosition(CurrentUpperBodyAttackMontage);
		FName CurrentSection = Montage_GetCurrentSection(CurrentUpperBodyAttackMontage);
				
		if (bShowSwapHitMontageLogs)
		{
			UE_LOG(LogSSIMAnimation, Warning, TEXT("%s | CURRENT Upper Body Attack Montage SECTION: %s"), TEXT(__FUNCTION__), *CurrentSection.ToString());
			UE_LOG(LogSSIMAnimation, Warning, TEXT("%s | CURRENT Upper Body Attack Montage POSITION: %f"), TEXT(__FUNCTION__), CurrentAttackMontagePosition);
			UE_LOG(LogSSIMAnimation, Warning, TEXT("%s | FULL Body Attack Montage: %s"), TEXT(__FUNCTION__), *FullBodyAttackMontage->GetName());
			UE_LOG(LogSSIMAnimation, Warning, TEXT("%s | CURRENT Upper Body Attack Montage: %s"), TEXT(__FUNCTION__), *CurrentUpperBodyAttackMontage->GetName());
		}
		
		Montage_Stop(0.01f, CurrentUpperBodyAttackMontage);
		if (bShowSwapHitMontageLogs)
		{
			UE_LOG(LogSSIMAnimation, Warning, TEXT("%s | Montage STOP"), TEXT(__FUNCTION__));
		}
		
		Montage_Play(FullBodyAttackMontage, 0.6f, EMontagePlayReturnType::MontageLength, CurrentAttackMontagePosition);
		if (bShowSwapHitMontageLogs)
		{
			UE_LOG(LogSSIMAnimation, Warning, TEXT("%s | Montage PLAY"), TEXT(__FUNCTION__));
		}
		
		Montage_JumpToSection(CurrentSection, FullBodyAttackMontage);
		if (bShowSwapHitMontageLogs)
		{
			UE_LOG(LogSSIMAnimation, Warning, TEXT("%s | Montage JUMP TO SECTION"), TEXT(__FUNCTION__));
		}
		
		Montage_SetPosition(FullBodyAttackMontage, CurrentAttackMontagePosition);
		if (bShowSwapHitMontageLogs)
		{
			UE_LOG(LogSSIMAnimation, Warning, TEXT("%s | Montage SET POSITION"), TEXT(__FUNCTION__));
		}
	}
}

void USSIMPlayerAnimInstance::OnHitRegistrationHandle(EPlayerAttackDirectionType InPlayerAttackDirectionType)
{
	SwapHitMontage(InPlayerAttackDirectionType);
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

void USSIMPlayerAnimInstance::OnDamageReceivedHandler(const FDamageData& InDamageData)
{
	if (bShowPogoBlendLogs)
	{
		UE_LOG(LogSSIMAnimation, Error, TEXT("%s"), TEXT(__FUNCTION__));
	}
	
	StartBlendOutPogoBones(OnPogoInterruptedBlendOutDuration);
}

void USSIMPlayerAnimInstance::OnHealingStartedHandler()
{
	if (bShowPogoBlendLogs)
	{
		UE_LOG(LogSSIMAnimation, Error, TEXT("%s"), TEXT(__FUNCTION__));
	}
	
	StartBlendOutPogoBones(OnPogoInterruptedBlendOutDuration);
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

void USSIMPlayerAnimInstance::OnDashStartedHandler()
{
	if (bShowPogoBlendLogs)
	{
		UE_LOG(LogSSIMAnimation, Error, TEXT("%s"), TEXT(__FUNCTION__));
	}
	
	StartBlendOutPogoBones(0.11f);
}
