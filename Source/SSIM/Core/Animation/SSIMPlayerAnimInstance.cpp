// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerAnimInstance.h"

#include "SSIM/SSIM.h"
#include "SSIM/Characters/Player/SSIMPlayer.h"
#include "SSIM/Components/Combat/SSIMPlayerCombatComponent.h"
#include "SSIM/Components/Stats/SSIMPlayerStatsComponent.h"

void USSIMPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	SSIMPlayer->GetPlayerCombatComponent()->OnPogoAnimationStartedDelegate.AddDynamic(this, &USSIMPlayerAnimInstance::OnPogoAnimationStartedHandler);
	SSIMPlayer->GetPlayerCombatComponent()->OnPogoEndedDelegate.AddDynamic(this, &USSIMPlayerAnimInstance::OnPogoEndedHandler);
	SSIMPlayer->GetPlayerCombatComponent()->OnPogoAnimationEndedDelegate.AddDynamic(this, &USSIMPlayerAnimInstance::OnPogoAnimationEndedHandler);
	
	SSIMPlayer->GetPlayerStatsComponent()->OnDamageReceivedDelegate.AddDynamic(this, &USSIMPlayerAnimInstance::OnDamageReceivedHandler);
	SSIMPlayer->LandedDelegate.AddDynamic(this, &USSIMPlayerAnimInstance::OnPlayerLanded);
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

void USSIMPlayerAnimInstance::ModifyBonesForPogo()
{
	PogoAdditionalRotation_Pelvis   = PogoAdditionalDeltaRotation_Pelvis;
	PogoAdditionalRotation_Spine_01 = PogoAdditionalDeltaRotation_Spine_01;
	PogoAdditionalRotation_Spine_02 = PogoAdditionalDeltaRotation_Spine_02;
	PogoAdditionalRotation_Thigh_L  = PogoAdditionalDeltaRotation_Thigh_L;
	PogoAdditionalRotation_Thigh_R  = PogoAdditionalDeltaRotation_Thigh_R;
}

void USSIMPlayerAnimInstance::ResetBonesAfterPogo()
{
	PogoAdditionalRotation_Pelvis   = FRotator::ZeroRotator;
	PogoAdditionalRotation_Spine_01 = FRotator::ZeroRotator;
	PogoAdditionalRotation_Spine_02 = FRotator::ZeroRotator;
	PogoAdditionalRotation_Thigh_L  = FRotator::ZeroRotator;
	PogoAdditionalRotation_Thigh_R  = FRotator::ZeroRotator;
}

void USSIMPlayerAnimInstance::OnPogoAnimationStartedHandler()
{
	ModifyBonesForPogo();
}

void USSIMPlayerAnimInstance::OnPogoEndedHandler()
{
	ResetBonesAfterPogo();
}

void USSIMPlayerAnimInstance::OnDamageReceivedHandler(const FDamageData& InDamageData)
{
	ResetBonesAfterPogo();
}

void USSIMPlayerAnimInstance::OnPogoAnimationEndedHandler()
{
	ResetBonesAfterPogo();
}

void USSIMPlayerAnimInstance::OnPlayerLanded(const FHitResult& Hit)
{
	ResetBonesAfterPogo();
}
