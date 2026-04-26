// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMDebugUtils.h"

#if !UE_BUILD_SHIPPING
#include "Kismet/GameplayStatics.h"
#include "SSIM/Characters/Player/SSIMPlayer.h"
#include "SSIM/Components/Stats/SSIMPlayerStatsComponent.h"

void USSIMDebugUtils::PrintPlayerStates(const UObject* WorldContextObject)
{
	ASSIMPlayer* SSIMPlayer = Cast<ASSIMPlayer>(UGameplayStatics::GetPlayerCharacter(WorldContextObject, 0));
	if (!SSIMPlayer)
	{
		return;
	}
	USSIMPlayerStatsComponent* PlayerStatsComponent = SSIMPlayer->GetPlayerStatsComponent();
	
	bool bAttacking    = SSIMPlayer->bAttacking;
	bool bPogoActive   = SSIMPlayer->bPogoActive;
	bool bDashing	   = SSIMPlayer->bDashing;
	bool bCanDash	   = SSIMPlayer->bCanDash;
	bool bStaggered	   = SSIMPlayer->bStaggered;
	bool bInvulnerable = PlayerStatsComponent->bInvulnerable;
	
	
	GEngine->AddOnScreenDebugMessage(
		-1, 
		0.f, 
		FColor::Red, 
		FString::Printf(TEXT("bAttacking: %s"), bAttacking ? TEXT("true") : TEXT("false")), 
		true,
		FVector2D(1.5f, 1.5f));
	
	GEngine->AddOnScreenDebugMessage(
		-1, 
		0.f, 
		FColor::Orange, 
		FString::Printf(TEXT("bPogoActive: %s"), bPogoActive ? TEXT("true") : TEXT("false")), 
		true,
		FVector2D(1.5f, 1.5f));
	
	GEngine->AddOnScreenDebugMessage(
		-1, 
		0.f, 
		FColor::Blue, 
		FString::Printf(TEXT("bDashing: %s"), bDashing ? TEXT("true") : TEXT("false")), 
		true,
		FVector2D(1.5f, 1.5f));
	
	GEngine->AddOnScreenDebugMessage(
		-1, 
		0.f, 
		FColor::Cyan, 
		FString::Printf(TEXT("bCanDash: %s"), bCanDash ? TEXT("true") : TEXT("false")), 
		true,
		FVector2D(1.5f, 1.5f));
	
	GEngine->AddOnScreenDebugMessage(
		-1, 
		0.f, 
		FColor::Magenta, 
		FString::Printf(TEXT("bStaggered: %s"), bStaggered ? TEXT("true") : TEXT("false")), 
		true,
		FVector2D(1.5f, 1.5f));
	
	GEngine->AddOnScreenDebugMessage(
		-1, 
		0.f, 
		FColor::Emerald, 
		FString::Printf(TEXT("bInvulnerable: %s"), bInvulnerable ? TEXT("true") : TEXT("false")), 
		true,
		FVector2D(1.5f, 1.5f));
}

#endif
