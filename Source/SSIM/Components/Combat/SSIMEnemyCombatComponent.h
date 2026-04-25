// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMBaseCombatComponent.h"

#include "SSIMEnemyCombatComponent.generated.h"

class ASSIMBaseEnemy;
class ASSIMPlayer;


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SSIM_API USSIMEnemyCombatComponent : public USSIMBaseCombatComponent
{
	GENERATED_BODY()
	
// Variables
#pragma region Stats
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Combat|Attack", 
			  meta = (ClampMin = 0, ClampMax = 3, UIMin = 0, UIMax = 3))
	int32 RegularAttackDamage = 1;
	
#pragma endregion Stats	
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Combat")
	EEnemyState EnemyState;
	
protected:
	UPROPERTY()
	TObjectPtr<ASSIMBaseEnemy> SSIMEnemy;
	
	UPROPERTY()
	TObjectPtr<APawn> PlayerPawn;

private:
	FTimerHandle ContinuousAttackDamageTimerHandle;
	FTimerDelegate ContinuousAttackDamageTimerDelegate;
	
	FTimerHandle ContinuousContactDamageTimerHandle;
	FTimerDelegate ContinuousContactDamageTimerDelegate;


// Overriden Functions
protected:
	virtual void BeginPlay() override;
	
// My Functions
public:
	virtual void EndAttack() override;
	
protected:
	virtual void SetReferences() override;
	
	virtual UAnimMontage* GetAttackMontage() override;
	
#pragma region Attack Damage
	
protected:
	virtual void OnAttackCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent,
									   		   AActor* OtherActor,
									   		   UPrimitiveComponent* OtherComp,
									   		   int32 OtherBodyIndex,
									   		   bool bFromSweep,
									   		   const FHitResult& SweepResult) override;

private:
	void StartContinuousAttackDamage();
	void EndContinuousAttackDamage();
	
#pragma endregion Attack Damage
	
#pragma region Contact Damage
private:
	UFUNCTION()
	void OnContactDamageCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent,
											  AActor* OtherActor,
											  UPrimitiveComponent* OtherComp,
											  int32 OtherBodyIndex,
											  bool bFromSweep,
											  const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnContactDamageCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent,
										    AActor* OtherActor,
										    UPrimitiveComponent* OtherComp,
										    int32 OtherBodyIndex);
	
	void StartContinuousContactDamage();
	void EndContinuousContactDamage();

#pragma endregion Contact Damage

	UFUNCTION()
	virtual void DealDamageToPlayer(UShapeComponent* DamageCollision);
};
