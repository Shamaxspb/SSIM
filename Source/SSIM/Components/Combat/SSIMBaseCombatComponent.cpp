// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMBaseCombatComponent.h"

#include "Components/BoxComponent.h"
#include "SSIM/SSIM.h"


void USSIMBaseCombatComponent::StartAttack()
{
	if (bAttacking)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Attack is still in process"), TEXT(__FUNCTION__));
		return;
	}
	
	if (!IsValid(GetAttackMontage()))
	{
		// Logging inside GetAttackMontage()
		return;
	}
	AttackMontage = GetAttackMontage();
	
	bAttacking = true; // set to false in ANS_AttackProcessing::NotifyEnd, so ANS_AttackProcessing MUST be in AnimMontage
	
	AnimInstance->Montage_Play(AttackMontage);
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Attack started"), TEXT(__FUNCTION__));
}

void USSIMBaseCombatComponent::EndAttack()
{
	bAttacking = false;
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Attack ended"), TEXT(__FUNCTION__));
	HitEnemies.Empty();
}

void USSIMBaseCombatComponent::StartAttackTrace()
{
	if (!IsValid(CurrentAttackCollision))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | CurrentAttackCollision is not valid"), TEXT(__FUNCTION__));
		return;
	}
	CurrentAttackCollision->OnComponentBeginOverlap.AddDynamic(this, &USSIMBaseCombatComponent::OnAttackCollisionBeginOverlap);
	
	CurrentAttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CurrentAttackCollision->UpdateOverlaps();
	
	#if !UE_BUILD_SHIPPING
	CurrentAttackCollision->SetHiddenInGame(false);
	#endif
	
	
	//UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Activated Attack Collision: %s"), TEXT(__FUNCTION__), *CurrentAttackCollision->GetName());
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Attack trace STARTED"), TEXT(__FUNCTION__));
}

void USSIMBaseCombatComponent::EndAttackTrace()
{
	if (!CurrentAttackCollision)
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | Couldn't receive Attack Collision to deactivate"), TEXT(__FUNCTION__));
		return;
	}
	
	CurrentAttackCollision->OnComponentBeginOverlap.RemoveDynamic(this, &USSIMBaseCombatComponent::OnAttackCollisionBeginOverlap);
	CurrentAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	#if !UE_BUILD_SHIPPING
	CurrentAttackCollision->SetHiddenInGame(true);
	#endif

	HitEnemies.Empty();
	
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Attack trace ENDED"), TEXT(__FUNCTION__));
}

UAnimMontage* USSIMBaseCombatComponent::GetAttackMontage()
{
	UE_LOG(LogSSIMInheritance, Error, TEXT("%s | GetAttackMontage() is not overriden"), *GetOwner()->GetName());
	return nullptr;
}

void USSIMBaseCombatComponent::OnAttackCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent,
															 AActor* OtherActor, 
															 UPrimitiveComponent* OtherComp, 
															 int32 OtherBodyIndex, 
															 bool bFromSweep,
															 const FHitResult& SweepResult)
{
	UE_LOG(LogSSIMInheritance, Error, TEXT("%s | OnAttackCollisionBeginOverlap() is not overriden"), *GetOwner()->GetName());
	return;
}
