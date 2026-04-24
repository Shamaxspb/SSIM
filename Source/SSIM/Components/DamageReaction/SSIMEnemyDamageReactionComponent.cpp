// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMEnemyDamageReactionComponent.h"

#include "SSIM/Components/Stats/SSIMEnemyStatsComponent.h"
#include "SSIM/Core/Types/SSIMCombatDataTypes.h"

//


void USSIMEnemyDamageReactionComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void USSIMEnemyDamageReactionComponent::SetReferences()
{
	Super::SetReferences();
	
	EnemyStatsComponent = Cast<USSIMEnemyStatsComponent>(BaseStatsComponent);
}

void USSIMEnemyDamageReactionComponent::OnDamageReceivedHandler(FDamageData DamageData)
{
	Super::OnDamageReceivedHandler(DamageData);
	
	EnemyStatsComponent->EnemyState = EEnemyState::EES_Staggered;
}
