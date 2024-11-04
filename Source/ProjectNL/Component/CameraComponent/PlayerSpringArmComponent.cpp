// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectNL/Component/CameraComponent/PlayerSpringArmComponent.h"

UPlayerSpringArmComponent::UPlayerSpringArmComponent()
{
}

void UPlayerSpringArmComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerSpringArmComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
