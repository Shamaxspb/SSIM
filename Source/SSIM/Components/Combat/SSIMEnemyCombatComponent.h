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
	
	UPROPERTY()
	TObjectPtr<ASSIMBaseEnemy> SSIMEnemy;
	
protected:
	UPROPERTY()
	TObjectPtr<APawn> PlayerPawn;

// Overriden Functions
	virtual void BeginPlay() override;
	
// My Functions
protected:
	virtual void OnAttackCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent,
									   		   AActor* OtherActor,
									   		   UPrimitiveComponent* OtherComp,
									   		   int32 OtherBodyIndex,
									   		   bool bFromSweep,
									   		   const FHitResult& SweepResult) override;
	
	virtual void SetReferences() override;
	
private:
	virtual void DealDamageToPlayer();

};
