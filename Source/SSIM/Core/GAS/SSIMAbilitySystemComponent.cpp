// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMAbilitySystemComponent.h"



USSIMAbilitySystemComponent::USSIMAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void USSIMAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void USSIMAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

