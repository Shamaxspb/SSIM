// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerInputComponent.h"


USSIMPlayerInputComponent::USSIMPlayerInputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void USSIMPlayerInputComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void USSIMPlayerInputComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}
