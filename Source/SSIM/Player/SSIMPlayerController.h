// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMPlayer.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "SSIMPlayerController.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UInputMappingContext;

UCLASS()
class SSIM_API ASSIMPlayerController : public APlayerController
{
	GENERATED_BODY()
	
// Variables
protected:
	UPROPERTY(BlueprintReadWrite, DisplayName = "Player", Category = "SSIM|References")
	TObjectPtr<ASSIMPlayer> SSIMPlayer = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SSIM|ToOverride")
	UInputMappingContext* BaseInputContext;
	
private:
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem;
	
// Overriden Functions
public:
	ASSIMPlayerController();

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;
	
public:
	virtual void Tick(float DeltaTime) override;

	
// My Functions
protected:
	UFUNCTION(BlueprintCallable, Category = "SSIM|Movement")
	void MovementProcessing() const;
	
	UFUNCTION(BlueprintCallable, Category = "SSIM|Abilities")
	virtual void PerformDash() const;
	
private:
	virtual void Init(); 
	void GetSSIMPlayerReference(APawn *InPawn);
	void InitBasicInputContext();
	
	

};
