// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIM/Components/SSIMBaseComponent.h"
#include "SSIM/Core/Types/SSIMCombatDataTypes.h"

#include "SSIMBaseCombatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackStartedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEndedSignature);

class UBoxComponent;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SSIM_API USSIMBaseCombatComponent : public USSIMBaseComponent
{
	GENERATED_BODY()
	
	
// Variables
public:
	FOnAttackStartedSignature OnAttackStartedDelegate;
	FOnAttackEndedSignature OnAttackEndedDelegate;
	
#pragma region Metadata

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations|Attack")
	TObjectPtr<UAnimMontage> AttackMontage;
	
	UPROPERTY(BlueprintReadOnly, Category = "SSIM|Components|Attack")
	UBoxComponent* CurrentAttackCollision;
	
	UPROPERTY()
	TArray<AActor*> HitEnemies;
	
	FDamageData DamageData;
		
#pragma endregion Metadata

#pragma region Debug

protected:
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Debug")
	bool bShowDebug;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Debug", meta = (EditCondition = "bShowDebug", EditConditionHides))
	bool bShowLogs;

#pragma endregion Debug
		
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
