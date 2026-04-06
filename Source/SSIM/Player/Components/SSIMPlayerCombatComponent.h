// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMBasePlayerComponent.h"
#include "Components/ActorComponent.h"
#include "SSIMPlayerCombatComponent.generated.h"

class ASSIMPlayer;
class UBoxComponent;

UCLASS(Blueprintable, ClassGroup=(PlayerComponents))
class SSIM_API USSIMPlayerCombatComponent : public USSIMBasePlayerComponent
{
	GENERATED_BODY()

// Variables
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations")
	TArray<UAnimMontage*> PlayerAttackAnimations;

private:
	bool bIsAttacking = false;
	
	// UPROPERTY(EditDefaultsOnly, Category = "SSIM|Collision")
	// TObjectPtr<UBoxComponent> FrontalAttackCollision;
	//
	// UPROPERTY(EditDefaultsOnly, Category = "SSIM|Collision")
	// TObjectPtr<UBoxComponent> UpperAttackCollision;
	//
	// UPROPERTY(EditDefaultsOnly, Category = "SSIM|Collision")
	// TObjectPtr<UBoxComponent> BottomAttackCollision; 
	
// Overriden Functions
public:
	USSIMPlayerCombatComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

// My Functions
public:
	UFUNCTION(BlueprintCallable, Category = "SSIM|Combat")
	void StartAttack();
	void EndAttack();

private:
	void ValidateAttackCollisionComponents(); 
};
