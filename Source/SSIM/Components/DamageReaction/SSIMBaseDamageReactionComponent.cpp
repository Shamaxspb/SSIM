// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMBaseDamageReactionComponent.h"

#include "GameFramework/Character.h"
#include "SSIM/Components/Stats/SSIMPlayerStatsComponent.h"


void USSIMBaseDamageReactionComponent::BeginPlay()
{
	Super::BeginPlay();
	SetReferences();

	BaseStatsComponent->OnDamageReceivedDelegate.AddDynamic(this, &USSIMBaseDamageReactionComponent::OnDamageReceivedHandler);
}

void USSIMBaseDamageReactionComponent::SetReferences()
{
	Super::SetReferences();
	
	BaseStatsComponent = SSIMOwnerCharacter->FindComponentByClass<USSIMBaseStatsComponent>();
}

void USSIMBaseDamageReactionComponent::OnDamageReceivedHandler(FDamageData DamageData)
{
	
}
