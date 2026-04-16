// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMPlayer.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "SSIMPlayerController.generated.h"

class USSIMHealthBar;
class USSIMBaseUI;
class UEnhancedInputLocalPlayerSubsystem;
class UInputMappingContext;
class USSIMPlayerCombatComponent;
class USSIMPlayerFlowComponent;

UCLASS()
class SSIM_API ASSIMPlayerController : public APlayerController
{
	GENERATED_BODY()
	
// Variables
#pragma region References
	
protected:
	UPROPERTY(BlueprintReadWrite, DisplayName = "Player", Category = "SSIM|References")
	TObjectPtr<ASSIMPlayer> SSIMPlayer;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Components", DisplayName = "PlayerCombatComponent")
	TObjectPtr<USSIMPlayerCombatComponent> SSIMPlayerCombatComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Components", DisplayName = "PlayerFlowComponent")
	TObjectPtr<USSIMPlayerFlowComponent> SSIMPlayerFlowComponent;

#pragma endregion References

#pragma region UI
	
	UPROPERTY()
	TObjectPtr<USSIMHealthBar> HealthBarWidget;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SSIM|UI")
	TSubclassOf<USSIMHealthBar> HealthBarWidgetClass;
	
#pragma endregion UI
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SSIM|ToOverride")
	UInputMappingContext* BaseInputContext;
	
private:
	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem;
	
// Overriden Functions
public:
	ASSIMPlayerController();

	virtual void BeginPlay() override;
	
protected:
	virtual void OnPossess(APawn* InPawn) override;
	
	
// My Functions	
private:
	virtual void Init(); 
	void SetPlayerReference(APawn *InPawn);
	void SetActorComponentsReferences();
	void InitBasicInputContext();
	void InitUI();
	
};
