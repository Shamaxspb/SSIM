// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMEnemyCombatComponent.h"

#include "Components/CapsuleComponent.h"
#include "SSIM/SSIM.h"
#include "SSIM/Characters/Enemies/SSIMBaseEnemy.h"
#include "SSIM/Core/Interfaces/SSIMPlayerCombatInterface.h"


// Overriden Functions
void USSIMEnemyCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SetReferences();
	SSIMEnemy->GetContactDamageCollision()->OnComponentBeginOverlap.AddDynamic(this, &USSIMEnemyCombatComponent::OnContactDamageCollisionBeginOverlap);
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

void USSIMEnemyCombatComponent::OnContactDamageCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | %s collided with : %s"), TEXT(__FUNCTION__), *SSIMOwnerCharacter->GetName(), *OtherActor->GetName());
	if (OtherActor == PlayerPawn)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Hit Character is Player"), TEXT(__FUNCTION__));
		DealDamageToPlayer();
	}
}

void USSIMEnemyCombatComponent::SetReferences()
{
	Super::SetReferences();
	
	SSIMEnemy = CastChecked<ASSIMBaseEnemy>(SSIMOwnerCharacter);
	CurrentAttackCollision = SSIMEnemy->GetAttackCollision();
	
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void USSIMEnemyCombatComponent::DealDamageToPlayer()
{
	if (!PlayerPawn->Implements<USSIMDamageableInterface>())
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | PlayerPawn doesn't implement USSIMDamageableInterface"), TEXT(__FUNCTION__));
		return;
	}
	
	DamageData.DamageInstigator = SSIMOwnerCharacter;
	DamageData.DamageValue = RegularAttackDamage;
	
	ISSIMDamageableInterface::Execute_ReceiveDamageInterface(PlayerPawn, DamageData);
}
