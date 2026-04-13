// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMEnemyCombatComponent.h"


USSIMEnemyCombatComponent::USSIMEnemyCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void USSIMEnemyCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void USSIMEnemyCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

