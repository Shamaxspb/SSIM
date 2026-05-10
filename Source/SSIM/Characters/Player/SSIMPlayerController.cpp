// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerController.h" 

#include "SSIM/SSIM.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "SSIMPlayer.h"
#include "Blueprint/UserWidget.h"
#include "SSIM/Core/UI/SSIMHealthBar.h"


// Overriden Functions
ASSIMPlayerController::ASSIMPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASSIMPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	Init();
	
	SSIMPlayer = CastChecked<ASSIMPlayer>(GetPawn());
	SSIMPlayer->GetPlayerStatsComponent()->OnCharacterDiedDelegate.AddDynamic(this, &ASSIMPlayerController::OnCharacterDiedHandler);
}

// My Functions
void ASSIMPlayerController::Init()
{
	InitBasicInputContext();
	InitUI();
	
}

void ASSIMPlayerController::InitBasicInputContext()
{
	if (!GetLocalPlayer())
	{
		UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("%s : LocalPlayer is not valid"), TEXT(__FUNCTION__));
		return;
	}
	
	EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!EnhancedInputSubsystem)
	{
		UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("%s : EnhancedInputSubsystem is not valid"), TEXT(__FUNCTION__));
		return;
	}
	
	EnhancedInputSubsystem->AddMappingContext(BaseInputContext, 1);
}

void ASSIMPlayerController::InitUI()
{
	// Health Bar
	if (!IsValid(HealthBarWidgetClass))
	{
		UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("%s : HealthBarWidget class is not valid"), TEXT(__FUNCTION__));
		return;
	}
	HealthBarWidget = CreateWidget<USSIMHealthBar>(this, HealthBarWidgetClass, TEXT("HealthBarWidget"));
	HealthBarWidget->AddToViewport();
}

void ASSIMPlayerController::OnCharacterDiedHandler()
{
	EnhancedInputSubsystem->RemoveMappingContext(BaseInputContext);
}
