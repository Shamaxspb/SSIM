// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMBaseEnemy.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "SSIM/Components/Combat/SSIMEnemyCombatComponent.h"
#include "SSIM/Components/Stats/SSIMEnemyStatsComponent.h"
#include "SSIM/Components/DamageReaction/SSIMEnemyDamageReactionComponent.h"

// Overriden Functions
ASSIMBaseEnemy::ASSIMBaseEnemy()
{
	GetCapsuleComponent()->SetCollisionProfileName("Enemy", true);
	GetMesh()->SetCollisionProfileName("Enemy", true);
	
	EnemyCombatComponent		  = CreateDefaultSubobject<USSIMEnemyCombatComponent>(TEXT("CombatComponent"));
	EnemyStatsComponent			  = CreateDefaultSubobject<USSIMEnemyStatsComponent>(TEXT("StatsComponent"));
	EnemyDamageReactionComponent  = CreateDefaultSubobject<USSIMEnemyDamageReactionComponent>(TEXT("DamageReactionComponent"));
	
	SetupAttackCollision();
}

void ASSIMBaseEnemy::SetupAttackCollision()
{
	AttackBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollision"));
	AttackBoxCollision->SetupAttachment(GetRootComponent());
	
	AttackBoxCollision->SetGenerateOverlapEvents(true);
	AttackBoxCollision->SetCollisionProfileName("MeleeAttack", true);
	AttackBoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


// Interfaces
void ASSIMBaseEnemy::StartAttackInterface_Implementation() const
{
	EnemyCombatComponent->StartAttack();
}

void ASSIMBaseEnemy::EndAttackInterface_Implementation() const
{
	EnemyCombatComponent->EndAttack();
}

void ASSIMBaseEnemy::StartAttackTraceInterface_Implementation() const
{
	EnemyCombatComponent->StartAttackTrace();
}

void ASSIMBaseEnemy::EndAttackTraceInterface_Implementation() const
{
	EnemyCombatComponent->EndAttackTrace();
}


void ASSIMBaseEnemy::ReceiveDamageInterface_Implementation(const FDamageData& InDamageData) const
{
	EnemyStatsComponent->ReduceHealth(InDamageData);
}
