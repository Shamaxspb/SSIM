// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMBaseCombatComponent.h"
#include "SSIM/Core/Types/SSIMCombatDataTypes.h"

#include "SSIMPlayerCombatComponent.generated.h"

class ASSIMPlayer;
class UBoxComponent;


UCLASS(Blueprintable, ClassGroup=(PlayerComponents))
class SSIM_API USSIMPlayerCombatComponent : public USSIMBaseCombatComponent
{
	GENERATED_BODY()

// Variables
	
#pragma region Stats
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Combat|Stats")
	float MeleeDamage;
	
#pragma endregion Stats
	
#pragma region Montages
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations|Attack")
	TArray<UAnimMontage*> PlayerFrontalAttackMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations|Attack")
	TArray<UAnimMontage*> PlayerAirFrontalAttackMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations|Attack")
	TArray<UAnimMontage*> PlayerUpwardAttackMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations|Attack")
	TArray<UAnimMontage*> PlayerAirUpwardAttackMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations|Attack")
	TArray<UAnimMontage*> PlayerDownwardAttackMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations|Attack")
	TArray<UAnimMontage*> PlayerAirDownwardAttackMontages;

#pragma endregion Montages
	
#pragma region Metadata
	
private:
	bool bIsAttacking = false;
	EPlayerAttackDirection PlayerAttackDirection;
	
	UPROPERTY()
	TSet<AActor*> HitCharacters;
	
	UPROPERTY()
	UBoxComponent* CurrentAttackCollision;
	
#pragma endregion Metadata
	
protected:
	UPROPERTY()
	TObjectPtr<ASSIMPlayer> SSIMPlayer;
	
	
// My Functions
public:
	
	UFUNCTION(BlueprintCallable, Category = "SSIM|Combat")
	void StartAttack();
	void StartAttackFrontal();
	void StartAttackUpward();
	void StartAttackDownward();
	void EndAttack();
	
	void StartAttackTrace();
	void EndAttackTrace();

protected:
	virtual void SetReferences() override;
	
private:
	UAnimMontage* GetAttackMontage() const;
	
	UFUNCTION()
	void OnAttackCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent,
									   AActor* OtherActor,
									   UPrimitiveComponent* OtherComp,
									   int32 OtherBodyIndex,
									   bool bFromSweep,
									   const FHitResult& SweepResult);
	
	void DealDamageToEnemy();
	
	
	
// DEBUG
	UFUNCTION(BlueprintCallable, Category = "SSIM|DEBUG")
	void SwitchAttackCollision_DEBUG() const;
};
