// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMBaseEnemy.h"

#include "SSIM/Enemies/Components/SSIMEnemyHealthComponent.h"

// Overriden Functions
ASSIMBaseEnemy::ASSIMBaseEnemy()
{
	EnemyHealthComponent = CreateDefaultSubobject<USSIMEnemyHealthComponent>(TEXT("HealthComponent"));
	// Combat Component
}


// My Functions


// Interfaces
void ASSIMBaseEnemy::ReceiveDamage_Implementation(float InDamage) const
{
	EnemyHealthComponent->ReduceHealth(InDamage);
}
