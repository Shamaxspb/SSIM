// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMKhaimeraEnemy.h"

#include "Components/ArrowComponent.h"


// Overriden Functions
ASSIMKhaimeraEnemy::ASSIMKhaimeraEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	if (GetArrowComponent())
	{
		GetArrowComponent()->SetHiddenInGame(false);
	}
	
}

void ASSIMKhaimeraEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void ASSIMKhaimeraEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
