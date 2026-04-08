// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMBasePlayerComponent.h"
#include "Components/ActorComponent.h"
#include "SSIM/Core/Types/SSIMCombatDataTypes.h"
#include "SSIMPlayerCombatComponent.generated.h"

class ASSIMPlayer;
class UBoxComponent;

UCLASS(Blueprintable, ClassGroup=(PlayerComponents))
class SSIM_API USSIMPlayerCombatComponent : public USSIMBasePlayerComponent
{
	GENERATED_BODY()

// Variables
public:
#pragma region Montages
	
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
	
	EPlayerAttackDirection PlayerAttackDirection;
	
	UPROPERTY()
	TSet<AActor*> HitActors;
	
private:
	bool bIsAttacking = false;
	
	UPROPERTY()
	UBoxComponent* CurrentAttackCollision;
	
// Overriden Functions
public:
	USSIMPlayerCombatComponent();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

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

private:
	UAnimMontage* GetAttackMontage() const;
	
	UFUNCTION()
	void OnAttackCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent,
									   AActor* OtherActor,
									   UPrimitiveComponent* OtherComp,
									   int32 OtherBodyIndex,
									   bool bFromSweep,
									   const FHitResult& SweepResult);
	
	/*UFUNCTION()
	void OnAttackCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent,
									 AActor* OtherActor,
									 UPrimitiveComponent* OtherComp,
									 int32 OtherBodyIndex,
									 bool bFromSweep,
									 const FHitResult& SweepResult);*/
	
// DEBUG
	UFUNCTION(BlueprintCallable, Category = "SSIM|DEBUG")
	void SwitchAttackCollision_DEBUG() const;
};
