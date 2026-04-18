// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMEnemyCombatComponent.h"

#include "SSIM/SSIM.h"
#include "SSIM/Characters/Enemies/SSIMBaseEnemy.h"
#include "SSIM/Core/Interfaces/SSIMPlayerCombatInterface.h"


// Overriden Functions
void USSIMEnemyCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SetReferences();
}

void USSIMEnemyCombatComponent::OnAttackCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                                              AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                              const FHitResult& SweepResult)
{
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Hit Character : %s"), TEXT(__FUNCTION__), *OtherActor->GetName());
	
	if (OtherActor == PlayerPawn)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Hit Character is Player"), TEXT(__FUNCTION__));
		DealDamageToPlayer();
	}
}

void USSIMEnemyCombatComponent::SetReferences()
{
	Super::SetReferences();
	
	SSIMEnemy = CastChecked<ASSIMBaseEnemy>(SSIMCharacter);
	CurrentAttackCollision = SSIMEnemy->GetAttackCollision();
	
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void USSIMEnemyCombatComponent::DealDamageToPlayer()
{
	if (!PlayerPawn->Implements<USSIMPlayerCombatInterface>())
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | PlayerPawn doesn't implement USSIMPlayerCombatInterface"), TEXT(__FUNCTION__));
		return;
	}
	ISSIMPlayerCombatInterface::Execute_ReceiveDamageInterface(PlayerPawn, RegularAttackDamage);
}
