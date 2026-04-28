// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMEnemyCombatComponent.h"

#include "SSIM/SSIM.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "SSIM/Characters/Enemies/SSIMBaseEnemy.h"


// Overriden Functions
void USSIMEnemyCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	SetReferences();
	
	SSIMEnemy->GetContactDamageCollision()->OnComponentBeginOverlap.AddDynamic(this, &USSIMEnemyCombatComponent::OnContactDamageCollisionBeginOverlap);
	SSIMEnemy->GetContactDamageCollision()->OnComponentEndOverlap.AddDynamic(this, &USSIMEnemyCombatComponent::OnContactDamageCollisionEndOverlap);

}

void USSIMEnemyCombatComponent::SetReferences()
{
	Super::SetReferences();
	
	SSIMEnemy = CastChecked<ASSIMBaseEnemy>(SSIMOwnerCharacter);
	CurrentAttackCollision = SSIMEnemy->GetAttackCollision();
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
}

UAnimMontage* USSIMEnemyCombatComponent::GetAttackMontage()
{
	if (!IsValid(AttackMontage))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | Attack Montage is not valid"), TEXT(__FUNCTION__));
		return nullptr;
	}
	return AttackMontage;
}

#pragma region AttackDamage 
void USSIMEnemyCombatComponent::OnAttackCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                                              AActor* OtherActor, 
                                                              UPrimitiveComponent* OtherComp, 
                                                              int32 OtherBodyIndex, 
                                                              bool bFromSweep,
                                                              const FHitResult& SweepResult)
{
	if (OtherActor == PlayerPawn)
	{
		StartContinuousAttackDamage();
	}
}

void USSIMEnemyCombatComponent::StartContinuousAttackDamage()
{
	ContinuousAttackDamageTimerDelegate.BindUObject(
						this,
						&USSIMEnemyCombatComponent::DealDamageToPlayer, 
						static_cast<UShapeComponent*>(CurrentAttackCollision));
	
	GetWorld()->GetTimerManager().SetTimer(
		ContinuousAttackDamageTimerHandle,
		ContinuousAttackDamageTimerDelegate,
		0.05f,
		true);
}

void USSIMEnemyCombatComponent::EndContinuousAttackDamage()
{
	GetWorld()->GetTimerManager().ClearTimer(ContinuousAttackDamageTimerHandle);
}

void USSIMEnemyCombatComponent::EndAttack()
{
	Super::EndAttack();
	
	EndContinuousAttackDamage();
}
#pragma endregion AttackDamage 

#pragma region ContactDamage 
void USSIMEnemyCombatComponent::OnContactDamageCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent,
																	 AActor* OtherActor, 
																	 UPrimitiveComponent* OtherComp, 
																	 int32 OtherBodyIndex, 
																	 bool bFromSweep,
																	 const FHitResult& SweepResult)
{
	if (OtherActor == PlayerPawn)
	{
		if (bShowAttackLogs)
		{
			UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | %s collided with : %s"), TEXT(__FUNCTION__), *SSIMOwnerCharacter->GetName(), *OtherActor->GetName());
		}
		StartContinuousContactDamage();
	}
}

void USSIMEnemyCombatComponent::OnContactDamageCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent,
																   AActor* OtherActor, 
																   UPrimitiveComponent* OtherComp, 
																   int32 OtherBodyIndex)
{
	EndContinuousContactDamage();
}


void USSIMEnemyCombatComponent::StartContinuousContactDamage()
{
	ContinuousContactDamageTimerDelegate.BindUObject(
						this,
						&USSIMEnemyCombatComponent::DealDamageToPlayer, 
						static_cast<UShapeComponent*>(SSIMEnemy->GetContactDamageCollision()));
	
	GetWorld()->GetTimerManager().SetTimer(
		ContinuousContactDamageTimerHandle,
		ContinuousContactDamageTimerDelegate,
		0.05f,
		true);
}

void USSIMEnemyCombatComponent::EndContinuousContactDamage()
{
	GetWorld()->GetTimerManager().ClearTimer(ContinuousContactDamageTimerHandle);
}
#pragma endregion ContactDamage 

void USSIMEnemyCombatComponent::DealDamageToPlayer(UShapeComponent* DamageCollision)
{
	if (!PlayerPawn->Implements<USSIMDamageableInterface>())
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | PlayerPawn doesn't implement USSIMDamageableInterface"), TEXT(__FUNCTION__));
		return;
	}
	
	if (bShowAttackLogs)
	{
		UE_LOG(LogSSIMGameplayMessages, Warning, TEXT("%s | %s : Try Deal Damage"), TEXT(__FUNCTION__), *DamageCollision->GetName());
	}
	
	if (DamageCollision->IsOverlappingActor(PlayerPawn))
	{
		DamageData.Instigator = SSIMOwnerCharacter;
		DamageData.Value = RegularAttackDamage;
		
		ISSIMDamageableInterface::Execute_ReceiveDamageInterface(PlayerPawn, DamageData);
	}
}
