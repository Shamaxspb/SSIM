// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SSIMBaseComponent.generated.h"

class ACharacter;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SSIM_API USSIMBaseComponent : public UActorComponent
{
	GENERATED_BODY()

// Variables
#pragma region References

protected:
	UPROPERTY()
	TObjectPtr<ACharacter> SSIMOwnerCharacter;
	
	UPROPERTY()
	TObjectPtr<UAnimInstance> AnimInstance;

#pragma endregion References
	
	
// Overriden Functions
public:
	USSIMBaseComponent();

	virtual void BeginPlay() override;
	
	
// My Functions
protected:
	virtual void SetReferences();
};
