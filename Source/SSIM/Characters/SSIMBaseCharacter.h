// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SSIMBaseCharacter.generated.h"

class UBoxComponent;


UCLASS(Abstract)
class SSIM_API ASSIMBaseCharacter : public ACharacter
{
	GENERATED_BODY()

// Variables
#pragma region Components

protected:
	// To group up components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Components|Damage", DisplayName = "DamageRegistrationGroup")
	TObjectPtr<USceneComponent> DamageRegistrationGroup;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Components|Damage", DisplayName = "HitRegistrationCollision")
	TObjectPtr<UCapsuleComponent> HitRegistrationCollision;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Components|Damage", DisplayName = "ContactDamageCollision")
	TObjectPtr<UCapsuleComponent> ContactDamageCollision;

#pragma endregion Components
	

// Overriden Functions
public:
	ASSIMBaseCharacter();
	
	virtual void OnConstruction(const FTransform& Transform) override;		
	
// My Functions
public:
	FORCEINLINE TObjectPtr<UCapsuleComponent> GetHitRegistrationCollision() const
	{
		return HitRegistrationCollision;
	}
	
	FORCEINLINE TObjectPtr<UCapsuleComponent> GetContactDamageCollision() const
	{
		return ContactDamageCollision;
	}
	
private:
	void RestrictXAxisMovement() const;
	void SetupDamageRegistrationCollision();
	
};
