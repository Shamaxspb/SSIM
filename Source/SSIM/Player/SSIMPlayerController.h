// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMPlayer.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "SSIMPlayerController.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UInputMappingContext;
class USSIMPlayerCombatComponent;
class USSIMPlayerFlowComponent;

UCLASS()
class SSIM_API ASSIMPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	// Variables
protected:
	UPROPERTY(BlueprintReadWrite, DisplayName = "Player", Category = "SSIM|References")
	TObjectPtr<ASSIMPlayer> SSIMPlayer = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Components", DisplayName = "PlayerCombatComponent")
	TObjectPtr<USSIMPlayerCombatComponent> SSIMPlayerCombatComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Components", DisplayName = "PlayerFlowComponent")
	TObjectPtr<USSIMPlayerFlowComponent> SSIMPlayerFlowComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SSIM|ToOverride")
	UInputMappingContext* BaseInputContext;
	
private:
	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem;
	
	// Overriden Functions
public:
	ASSIMPlayerController();

	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;
	
	
	// My Functions	
private:
	virtual void Init(); 
	void SetSSIMPlayerReference(APawn *InPawn);
	void SetSSIMActorComponentsReferences();
	void InitBasicInputContext();
	
};
