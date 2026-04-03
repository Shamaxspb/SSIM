// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMBasePlayerComponent.h"

#include "SSIM/SSIM.h"
#include "SSIM/Player/SSIMPlayer.h"


USSIMBasePlayerComponent::USSIMBasePlayerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void USSIMBasePlayerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetOwner())
	{
		SSIMPlayer = Cast<ASSIMPlayer>(GetOwner());
	}
	else
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s: GetOwner is not valid"), *this->GetName());
	}
	
}


void USSIMBasePlayerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

