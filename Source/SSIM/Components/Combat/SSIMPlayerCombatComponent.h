// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMBaseCombatComponent.h"
#include "SSIM/Core/Types/SSIMCombatDataTypes.h"

#include "SSIMPlayerCombatComponent.generated.h"

class ASSIMPlayer;
class UBoxComponent;
class USSIMPlayerStatsComponent;

UCLASS(Blueprintable, ClassGroup=(PlayerComponents))
class SSIM_API USSIMPlayerCombatComponent : public USSIMBaseCombatComponent
{
	GENERATED_BODY()

// Variables
#pragma region Stats
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Combat|Attack")
	float RegularAttackDamage;
	
#pragma endregion Stats	
	
#pragma region Montages
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations|Attack")
	TArray<TObjectPtr<UAnimMontage>> PlayerFrontalAttackMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations|Attack")
	TArray<TObjectPtr<UAnimMontage>> PlayerAirFrontalAttackMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations|Attack")
	TArray<TObjectPtr<UAnimMontage>> PlayerUpwardAttackMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations|Attack")
	TArray<TObjectPtr<UAnimMontage>> PlayerAirUpwardAttackMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations|Attack")
	TArray<TObjectPtr<UAnimMontage>> PlayerAirDownwardAttackMontages;

#pragma endregion Montages
	
#pragma region Metadata
	
public:
	EPlayerAttackDirectionType PlayerAttackDirectionType;
	
#pragma endregion Metadata
	
private:
	UPROPERTY()
	TObjectPtr<ASSIMPlayer> SSIMPlayer;
	
	UPROPERTY()
	TObjectPtr<USSIMPlayerStatsComponent> StatsComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Combat|Rebound On Downward Attack")
	float ReboundAngle = 50.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Combat|Rebound On Downward Attack")
	float ReboundVelocityZ = 400.f;

// Overriden Functions
protected:
	virtual void BeginPlay() override;

// My Functions
public:
	virtual void StartAttack() override;
	virtual void StartAttackTrace() override;

protected:
	virtual void SetReferences() override;
	virtual UAnimMontage* GetAttackMontage() override;
	
	virtual void OnAttackCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent,
											   AActor* OtherActor,
											   UPrimitiveComponent* OtherComp,
											   int32 OtherBodyIndex,
											   bool bFromSweep,
											   const FHitResult& SweepResult) override;
	
private:
	void DealDamageToEnemy();
	
	void ReboundOnDownwardAttack();
	
	UFUNCTION()
	void OnDamageReceivedHandler(const FDamageData& InDamageData);
	
};
