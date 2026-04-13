// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMBaseEnemy.h"

#include "SSIM/Components/Combat/SSIMEnemyCombatComponent.h"
#include "SSIM/Components/Stats/SSIMEnemyStatsComponent.h"

// Overriden Functions
ASSIMBaseEnemy::ASSIMBaseEnemy()
{
	EnemyCombatComponent = CreateDefaultSubobject<USSIMEnemyCombatComponent>(TEXT("CombatComponent"));
	EnemyStatsComponent  = CreateDefaultSubobject<USSIMEnemyStatsComponent>(TEXT("StatsComponent"));
}


// Interfaces
void ASSIMBaseEnemy::ReceiveDamage_Implementation(float InDamage) const
{
	EnemyStatsComponent->SetReceivedDamage(InDamage);
	EnemyStatsComponent->ReduceHealth();
}
