// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIM/Components/SSIMBaseComponent.h"
#include "SSIMBaseCombatComponent.generated.h"

class UBoxComponent;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SSIM_API USSIMBaseCombatComponent : public USSIMBaseComponent
{
	GENERATED_BODY()
// Variables
#pragma region Stats
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Combat|Stats")
	float RegularAttackDamage;
	
#pragma endregion Stats	
	
#pragma region Metadata

protected:
	bool bIsAttacking = false;
	
	UPROPERTY()
	TObjectPtr<UAnimMontage> AttackMontage = nullptr;
	
	UPROPERTY()
	UBoxComponent* CurrentAttackCollision;
	
	UPROPERTY()
	TSet<AActor*> HitCharacters;
		
#pragma endregion Metadata

	
// My Functions
public:
	virtual void StartAttack();
	virtual void EndAttack();
	virtual void StartAttackTrace();
	virtual void EndAttackTrace();
	
protected:
	virtual UAnimMontage* GetAttackMontage();
	
	virtual void OnAttackCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent,
									   AActor* OtherActor,
									   UPrimitiveComponent* OtherComp,
									   int32 OtherBodyIndex,
									   bool bFromSweep,
									   const FHitResult& SweepResult);
};
