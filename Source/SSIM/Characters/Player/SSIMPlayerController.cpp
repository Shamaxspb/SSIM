// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerController.h" 

#include "SSIMPlayer.h" 
#include "SSIM/SSIM.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "SSIM/Components/Combat/SSIMPlayerCombatComponent.h"
#include "SSIM/Components/PlayerComponents/SSIMPlayerFlowComponent.h"


// Overriden Functions
ASSIMPlayerController::ASSIMPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASSIMPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	Init();
	
}

// OnPossess called TWICE for some reason
// It doesn't affect anything yet, but should be considered in the future
void ASSIMPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	SetSSIMPlayerReference(InPawn);
	SetSSIMActorComponentsReferences();
	
}


// My Functions
void ASSIMPlayerController::Init()
{
	InitBasicInputContext();
	
}

void ASSIMPlayerController::SetSSIMPlayerReference(APawn* InPawn)
{
	if (!InPawn)
	{
		UE_LOG(LogSSIMPlayerInitialization, Log, TEXT("%s : Possessed pawn is not valid"), TEXT(__FUNCTION__));
	}
	
	UE_LOG(LogSSIMPlayerInitialization, Log, TEXT("%s: InPawn = %s"), TEXT(__FUNCTION__), *InPawn->GetName());

	SSIMPlayer = CastChecked<ASSIMPlayer>(InPawn);

	if (!IsValid(SSIMPlayer))
	{
		UE_LOG(LogSSIMPlayerInitialization, Log, TEXT("%s : Failed cast PlayerPawn to SSIMPlayer %s"), TEXT(__FUNCTION__), *InPawn->GetName());
		return;
	}
	
}

// Lost its purpose, won't delete for now
void ASSIMPlayerController::SetSSIMActorComponentsReferences()
{
	if (!SSIMPlayer)
	{
		UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("%s : SSIMPlayer is not valid"), TEXT(__FUNCTION__));
		return;
	}
	
	if (!IsValid(SSIMPlayer->GetPlayerFlowComponent()))
	{
		UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("%s : SSIMPlayerFlowComponent is not valid"), TEXT(__FUNCTION__))
	}
	SSIMPlayerFlowComponent = SSIMPlayer->GetPlayerFlowComponent();
	UE_LOG(LogSSIMPlayerInitialization, Log, TEXT("%s : %s is set"), TEXT(__FUNCTION__), *SSIMPlayerFlowComponent->GetName());
	
	if (!IsValid(SSIMPlayer->GetPlayerCombatComponent()))
	{
		UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("%s : SSIMPlayerCombatComponent is not valid"), TEXT(__FUNCTION__))
	}
	SSIMPlayerCombatComponent = SSIMPlayer->GetPlayerCombatComponent();
	UE_LOG(LogSSIMPlayerInitialization, Log, TEXT("%s : %s is set"), TEXT(__FUNCTION__), *SSIMPlayerCombatComponent->GetName());
	
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
	UE_LOG(LogSSIMPlayerInitialization, Log, TEXT("%s : EnhancedInputSubsystem %s added"), TEXT(__FUNCTION__), *BaseInputContext->GetName());
	
}
