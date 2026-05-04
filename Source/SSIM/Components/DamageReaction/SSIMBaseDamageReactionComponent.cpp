// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMBaseDamageReactionComponent.h"

#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SSIM/SSIM.h"
#include "SSIM/Components/Stats/SSIMPlayerStatsComponent.h"


void USSIMBaseDamageReactionComponent::BeginPlay()
{
	Super::BeginPlay();

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

void USSIMBaseDamageReactionComponent::ReboundOnHit()
{
	SSIMOwnerCharacter->LaunchCharacter(ReboundLaunchVelocity, true, true);

#if !UE_BUILD_SHIPPING
	
	if (bShowReboundLogs)
	{
		UE_LOG(LogSSIMGameplayMessages, Warning, TEXT("%s | ReboundLaunchVelocity: %s"), TEXT(__FUNCTION__), *ReboundLaunchVelocity.ToString());
	}
	if (bDrawReboundDebug)
	{
		ReboundDrawDebug();
	}
	
#endif !UE_BUILD_SHIPPING

	// Reset value for proper next Rebound calculation 
	ReboundLaunchVelocity = FVector::ZeroVector;
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
