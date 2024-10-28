// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectNL/Component/CameraComponent/PlayerCameraComponent.h"

UPlayerCameraComponent::UPlayerCameraComponent()
{
}

void UPlayerCameraComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
