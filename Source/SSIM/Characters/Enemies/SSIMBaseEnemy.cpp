// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMBaseEnemy.h"

#include "Components/BoxComponent.h"
#include "SSIM/Components/Combat/SSIMEnemyCombatComponent.h"
#include "SSIM/Components/Stats/SSIMEnemyStatsComponent.h"

// Overriden Functions
ASSIMBaseEnemy::ASSIMBaseEnemy()
{
	EnemyCombatComponent = CreateDefaultSubobject<USSIMEnemyCombatComponent>(TEXT("CombatComponent"));
	EnemyStatsComponent  = CreateDefaultSubobject<USSIMEnemyStatsComponent>(TEXT("StatsComponent"));
	
	SetupAttackCollision();
}

void ASSIMBaseEnemy::SetupAttackCollision()
{
	AttackBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollision"));
	AttackBoxCollision->SetupAttachment(GetRootComponent());
	
	AttackBoxCollision->SetGenerateOverlapEvents(true);
	AttackBoxCollision->SetCollisionProfileName("AttackTrace", true);
	AttackBoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


// Interfaces
void ASSIMBaseEnemy::ReceiveDamageInterface_Implementation(float InDamage) const
{
	EnemyStatsComponent->SetReceivedDamage(InDamage);
	EnemyStatsComponent->ReduceHealth();
}

void ASSIMBaseEnemy::StartAttackInterface_Implementation() const
{
	EnemyCombatComponent->StartAttack();
}

void ASSIMBaseEnemy::EndAttackInterface_Implementation() const
{
	EnemyCombatComponent->EndAttack();
}
