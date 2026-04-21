// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIM/Components/SSIMBaseComponent.h"
#include "SSIM/Core/Types/SSIMCombatDataTypes.h"

#include "SSIMBaseCombatComponent.generated.h"

class UBoxComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SSIM_API USSIMBaseCombatComponent : public USSIMBaseComponent
{
	GENERATED_BODY()
	
	
// Variables
#pragma region Metadata

protected:
	bool bIsAttacking = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations|Attack")
	TObjectPtr<UAnimMontage> AttackMontage;
	
	UPROPERTY(BlueprintReadOnly, Category = "SSIM|Components|Attack")
	UBoxComponent* CurrentAttackCollision;
	
	UPROPERTY()
	TSet<AActor*> HitEnemies;
	
	FDamageData DamageData;
		
#pragma endregion Metadata

		
// My Functions
public:
	virtual void StartAttack();
	virtual void EndAttack();
	virtual void StartAttackTrace();
	virtual void EndAttackTrace();
	
protected:
	UFUNCTION()
	virtual UAnimMontage* GetAttackMontage();
	
	UFUNCTION()
	virtual void OnAttackCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent,
											   AActor* OtherActor,
											   UPrimitiveComponent* OtherComp,
											   int32 OtherBodyIndex,
											   bool bFromSweep,
											   const FHitResult& SweepResult);
};
