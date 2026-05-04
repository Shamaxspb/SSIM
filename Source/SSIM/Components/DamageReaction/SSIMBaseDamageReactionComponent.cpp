// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMBaseDamageReactionComponent.h"

#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
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

void USSIMBaseDamageReactionComponent::OnDamageReceivedHandler(const FDamageData& InDamageData)
{
	DamageData.Instigator = InDamageData.Instigator;
	DamageData.Value = InDamageData.Value;
}

void USSIMBaseDamageReactionComponent::ReboundDrawDebug()
{
	UKismetSystemLibrary::DrawDebugArrow(GetWorld(), 
								GetOwner()->GetActorLocation(), 
								 GetOwner()->GetActorLocation() + ReboundLaunchVelocity.GetSafeNormal() * ReboundDirectionArrowLength, 
							   25.f, 
										 ReboundDirectionArrowColor, 
										 ReboundDrawDuration, 
							   5.f);
}
