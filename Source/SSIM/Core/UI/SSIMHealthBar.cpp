// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMHealthBar.h"

#include "SSIM/Characters/Player/SSIMPlayer.h"
#include "SSIM/Components/Stats/SSIMPlayerStatsComponent.h"

void USSIMHealthBar::Init()
{
	SetReferences();
	SSIMPlayer->GetPlayerStatsComponent()->OnDamageReceived.AddDynamic(this, &USSIMHealthBar::USSIMHealthBar::OnHealthChanged);
}

void USSIMHealthBar::SetReferences()
{
	PlayerStatsComponent = SSIMPlayer->GetPlayerStatsComponent();
}

void USSIMHealthBar::InitStatsValues()
{
	Health = PlayerStatsComponent->Health; // Should get from GameInstance for level transition in the future
	MaxHealth = PlayerStatsComponent->MaxHealth;
	
}
