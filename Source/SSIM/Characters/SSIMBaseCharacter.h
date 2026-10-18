// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "SSIM/Core/GAS/SSIMAbilitySystemComponent.h"

#include "SSIMBaseCharacter.generated.h"

class SSIMAbilitySystemComponent;
class USSIMCombatAttributeSet;
class UBoxComponent;


UCLASS(Abstract)
class SSIM_API ASSIMBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

// Variables
#pragma region Components
protected:
	// GAS
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|GAS", DisplayName = "AbilitySystemComponent")
	TObjectPtr<USSIMAbilitySystemComponent> SSIMAbilitySystemComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|GAS")
	TObjectPtr<USSIMCombatAttributeSet> SSIMAttributeSet;
	
	// To group up components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Components|Damage", DisplayName = "DamageRegistrationGroup")
	TObjectPtr<USceneComponent> DamageRegistrationGroup;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Components|Damage", DisplayName = "HitRegistrationCollision")
	TObjectPtr<UCapsuleComponent> HitRegistrationCollision;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Components|Damage", DisplayName = "ContactDamageCollision")
	TObjectPtr<UCapsuleComponent> ContactDamageCollision;
#pragma endregion Components
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> BasicAbilities;
	
	// Debug
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SSIM|Debug")
	bool bShowDebug;

// Overriden Functions
public:
	ASSIMBaseCharacter();
	
	virtual void BeginPlay() override;		
	
	virtual void OnConstruction(const FTransform& Transform) override;		
	
	virtual USSIMAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	
// Custom Functions
public:
	FORCEINLINE TObjectPtr<UCapsuleComponent> GetHitRegistrationCollision() const
	{
		return HitRegistrationCollision;
	}
	
	FORCEINLINE TObjectPtr<UCapsuleComponent> GetContactDamageCollision() const
	{
		return ContactDamageCollision;
	}
	
protected:
	virtual void InitializeAttributes();
	virtual void InitializeAbilities();
	
private:
	void RestrictXAxisMovement() const;
	void SetupDamageRegistrationCollision();
	
};
