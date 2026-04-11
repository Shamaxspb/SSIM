// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMKhaimeraEnemy.h"


ASSIMKhaimeraEnemy::ASSIMKhaimeraEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASSIMKhaimeraEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASSIMKhaimeraEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASSIMKhaimeraEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

