// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMBaseStatsComponent.h"

#include "SSIM/SSIM.h"


// My Functions
void USSIMBaseStatsComponent::ReduceHealth(const FDamageData& InDamageData)
{
	UE_LOG(LogSSIMInheritance, Error, TEXT("%s | ReduceHealth() is not overriden"), *GetOwner()->GetName());
}
