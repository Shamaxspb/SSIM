// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SSIMBaseEnemyComponent.generated.h"

class ASSIMBaseEnemy;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SSIM_API USSIMBaseEnemyComponent : public UActorComponent
{
	GENERATED_BODY()

// Variables
protected:
	UPROPERTY(BlueprintReadOnly, Category = "SSIM|References", DisplayName = "Enemy")
	ASSIMBaseEnemy* SSIMBaseEnemy;
	
	UPROPERTY()
	TObjectPtr<UAnimInstance> SSIMAnimInstance;

	
// Overriden Functions
public:
	USSIMBaseEnemyComponent();

protected:
	virtual void BeginPlay() override;
	
	
// My Functions
private:
	void SetReferences();
	
};
