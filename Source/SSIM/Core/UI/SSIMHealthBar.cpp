// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMHealthBar.h"

#include "SSIM/SSIM.h"
#include "SSIM/Characters/Player/SSIMPlayer.h"
#include "SSIM/Components/Stats/SSIMPlayerStatsComponent.h"

void USSIMHealthBar::NativeConstruct()
{
	Init();
	
	Super::NativeConstruct();
}

void USSIMHealthBar::Init()
{
	SetReferences();
	InitStatsValues();
	
	PlayerStatsComponent->OnDamageReceivedDelegate.AddDynamic(this, &USSIMHealthBar::OnDamageReceived);
	PlayerStatsComponent->OnHealReceivedDelegate.AddDynamic(this, &USSIMHealthBar::OnHealReceived);
}

void USSIMHealthBar::SetReferences()
{
	Super::SetReferences();
	
	if (!IsValid(SSIMPlayer))
	{
		UE_LOG(LogSSIMUIInitialization, Error, TEXT("%s | SSIMPlayer is not valid"), TEXT(__FUNCTION__));
		return;
	}
	PlayerStatsComponent = SSIMPlayer->GetPlayerStatsComponent();
}

void USSIMHealthBar::InitStatsValues()
{
	CachedHealth = PlayerStatsComponent->Health; // Should get from GameInstance for level transition in the future
	MaxHealth = PlayerStatsComponent->MaxHealth;
}
