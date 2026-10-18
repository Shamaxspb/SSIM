// Fill out your copyright notice in the Description page of Project Settings.

#include "SSIMGASHelper.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "SSIM/SSIM.h"
#include "SSIM/Characters/Player/SSIMPlayer.h"
#include "SSIM/Core/SSIMGameplayTags.h"
#include "SSIM/Core/Subsystems/Combat/SSIMCombatSubsystemSettings.h"

DEFINE_LOG_CATEGORY(LogSSIMGASHelper);

namespace SSIMGASHelper
{
	void ApplyDamageToTarget(AActor* InInstigator, AActor* InTarget, float InDamageAmount)
	{
		if (!IsValid(InInstigator))
		{
			UE_LOG(LogSSIMGASHelper, Error, TEXT("%s | Instigator is not valid"), TEXT(__FUNCTION__));
			return;
		}
		if (!IsValid(InTarget))
		{
			UE_LOG(LogSSIMGASHelper, Error, TEXT("%s | Target is not valid"), TEXT(__FUNCTION__));
			return;
		}
		UAbilitySystemComponent* InstigatorASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InTarget);
		if (!IsValid(InstigatorASC))
		{
			UE_LOG(LogSSIMGASHelper, Error, TEXT("%s | InstigatorASC is not valid"), TEXT(__FUNCTION__));
			return;
		}
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InTarget);
		if (!IsValid(TargetASC))
		{
			UE_LOG(LogSSIMGASHelper, Error, TEXT("%s | TargetASC is not valid"), TEXT(__FUNCTION__));
			return;
		}
		
		const auto* CombatSettings = GetDefault<USSIMCombatSubsystemSettings>();
		
		const FGameplayEffectContextHandle Context = InstigatorASC->MakeEffectContext();
		const FGameplayEffectSpecHandle SpecHandle = InstigatorASC->MakeOutgoingSpec(
			CombatSettings->DamageGE, 1.f, Context);
			
		if (!SpecHandle.IsValid())
		{
			UE_LOG(LogSSIMGASHelper, Error, TEXT("%s | SpecHandle is not valid"), TEXT(__FUNCTION__));
			return;
		}
		
		SpecHandle.Data->SetSetByCallerMagnitude(TAG_Attribute_Meta_Damage, InDamageAmount);
		TargetASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
	}
	
	void ApplyHealToPlayer(const UObject* WorldContextObject, int32 InHealAmount)
	{
		const ASSIMPlayer* Player = Cast<ASSIMPlayer>(UGameplayStatics::GetPlayerCharacter(WorldContextObject, 0));
		if (!Player)
		{
			UE_LOG(LogSSIMValidations, Error, TEXT("%s | Player is not valid"), TEXT(__FUNCTION__));
			return;
		}
		
		const auto* CombatSettings = GetDefault<USSIMCombatSubsystemSettings>();
		
		const FGameplayEffectContextHandle Context = Player->GetAbilitySystemComponent()->MakeEffectContext();
		const FGameplayEffectSpecHandle SpecHandle = Player->GetAbilitySystemComponent()->MakeOutgoingSpec(
			CombatSettings->HealGE, 1.f, Context);
		
		if (!SpecHandle.IsValid())
		{
			UE_LOG(LogSSIMGASHelper, Error, TEXT("%s | SpecHandle is not valid"), TEXT(__FUNCTION__));
			return;
		}
		
		const float Heal = static_cast<float>(InHealAmount);
		SpecHandle.Data->SetSetByCallerMagnitude(TAG_Attribute_Meta_Heal, Heal);
		Player->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}
