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
	
	bool bAttacking    			  = SSIMPlayer->IsPlayerAttacking();
	bool bPogoActive   			  = SSIMPlayer->IsPlayerPogoActive();
	bool bAttackKnockbackActive   = SSIMPlayer->IsPlayerAttackKnockbackActive();
	bool bStaggered	   			  = SSIMPlayer->IsPlayerStaggered();
	bool bInvulnerable 			  = SSIMPlayer->IsPlayerInvulnerable();
	
	
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
		FColor::Red, 
		FString::Printf(TEXT("bAttackKnockback: %s"), bAttackKnockbackActive ? TEXT("true") : TEXT("false")), 
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
