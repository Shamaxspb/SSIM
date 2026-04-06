// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMBasePlayerComponent.h"
#include "Components/ActorComponent.h"
#include "SSIMPlayerCombatComponent.generated.h"

enum class EPlayerAttackDirection : uint8;
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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations")
	TObjectPtr<UAnimMontage> PlayerAirAttackMontage;

private:
	bool bIsAttacking = false;
	
	UPROPERTY()
	UBoxComponent* CachedAttackCollision;
	
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
	
	void ActivateAttackCollision(EPlayerAttackDirection InPlayerAttackDirection);
	void DeactivateAttackCollision() const;

};
