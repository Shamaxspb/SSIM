// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerCombatComponent.h"
#include "SSIM/SSIM.h"
#include "SSIM/Player/SSIMPlayer.h"

// Overriden Functions
USSIMPlayerCombatComponent::USSIMPlayerCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void USSIMPlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void USSIMPlayerCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


// My Functions
void USSIMPlayerCombatComponent::StartAttack()
{
	if (bIsAttacking)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("Attack is still in process"));
		return;
	}
	
	if (PlayerAttackAnimations.IsEmpty())
	{
		UE_LOG(LogSSIMGameplayMessages, Error, TEXT("No Attack montages found"));
		return;
	}
	
	bIsAttacking = true;
	
	UAnimMontage* RandomAttackMontage = PlayerAttackAnimations[FMath::RandHelper(PlayerAttackAnimations.Num())];
	SSIMAnimInstance->Montage_Play(RandomAttackMontage);
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("Attack Montage: %s"), *RandomAttackMontage->GetName());
	
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("Attack started"));	
}

void USSIMPlayerCombatComponent::EndAttack()
{
	bIsAttacking = false;
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("Attack ended"));
}
