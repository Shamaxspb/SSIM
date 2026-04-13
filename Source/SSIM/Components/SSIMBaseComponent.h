// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SSIMBaseComponent.generated.h"

class ASSIMBaseEnemy;
class ASSIMPlayer;
class ACharacter;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SSIM_API USSIMBaseComponent : public UActorComponent
{
	GENERATED_BODY()

// Variables
#pragma region References

protected:
	UPROPERTY()
	TObjectPtr<ACharacter> SSIMCharacter;
	
	UPROPERTY()
	TObjectPtr<ASSIMPlayer> SSIMPlayer;
	
	UPROPERTY()
	TObjectPtr<ASSIMBaseEnemy> SSIMBaseEnemy;
	
	UPROPERTY()
	TObjectPtr<UAnimInstance> AnimInstance;

#pragma endregion References
	
	
// Overriden Functions
public:
	USSIMBaseComponent();

protected:
	virtual void BeginPlay() override;
	
	
// My Functions
protected:
	virtual void SetReferences();
};
