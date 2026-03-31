// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerFlowComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "SSIM/SSIM.h"
#include "SSIM/Player/SSIMPlayer.h"


// Overriden Functions
USSIMPlayerFlowComponent::USSIMPlayerFlowComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void USSIMPlayerFlowComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void USSIMPlayerFlowComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	if (GetOwner())
	{
		Player = Cast<ASSIMPlayer>(GetOwner());
	}
	else
	{
		UE_LOG(LogSSIMValidations, Log, TEXT("%s: GetOwner is not valid"), *this->GetName());
	}
	
}


void USSIMPlayerFlowComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


// My Functions
void USSIMPlayerFlowComponent::Dash() const
{
	/*if (bDashing)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("Dash is still in process"));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Dash is still in process"), true, FVector2D(1.4f, 1.4f));
		}
		return;
	}
	
	Player->LaunchCharacter(GetDashLaunchVelocity() ,true, false);*/
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("Dash started"));
}

FVector USSIMPlayerFlowComponent::GetDashLaunchVelocity() const
{
	FVector CurrentVelocity = Player->GetVelocity();
	FVector OutLaunchVelocity;
	
	if (CurrentVelocity.IsNearlyZero())
	{
		// Dash in place
		// Player->GetActorRotation();
		// UKismetMathLibrary::Vector_Right;
		
	}
	else
	{
		// Dash in motion
		OutLaunchVelocity = FVector(0.f, Player->GetVelocity().Y * DashVelocityCoef,0.f);
	}
		
	return OutLaunchVelocity;
}

