// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMKhaimeraEnemy.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "SSIM/SSIM.h"


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
	
	/*UE_LOG(LogSSIMPlayerInitialization, Warning, TEXT("%s || Attack Collision: %s | Collision Enabled: %s | GenerateOverlapEvents: %s"),
											    *GetName(),
												*AttackBoxCollision.GetName(), *UEnum::GetValueAsString(AttackBoxCollision->GetCollisionEnabled()),
												AttackBoxCollision->GetGenerateOverlapEvents() ? TEXT("True") : TEXT("False"));*/
}
