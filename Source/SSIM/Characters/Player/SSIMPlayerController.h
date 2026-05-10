// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"

#include "SSIMPlayerController.generated.h"

class ASSIMPlayer;
class USSIMHealthBar;
class UEnhancedInputLocalPlayerSubsystem;
class UInputMappingContext;

UCLASS()
class SSIM_API ASSIMPlayerController : public APlayerController
{
	GENERATED_BODY()
	
// Variables
#pragma region UI
	
protected:
	UPROPERTY()
	TObjectPtr<USSIMHealthBar> HealthBarWidget;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SSIM|UI")
	TSubclassOf<USSIMHealthBar> HealthBarWidgetClass;
	
#pragma endregion UI
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SSIM|ToOverride")
	UInputMappingContext* BaseInputContext;
	
private:
	UPROPERTY()
	TObjectPtr<ASSIMPlayer> SSIMPlayer;
	
	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem;
	
// Overriden Functions
public:
	ASSIMPlayerController();

protected:
	virtual void BeginPlay() override;
	
	
// My Functions	
private:
	virtual void Init(); 
	void InitBasicInputContext();
	void InitUI();
	
	UFUNCTION()
	void OnCharacterDiedHandler();
	
};
