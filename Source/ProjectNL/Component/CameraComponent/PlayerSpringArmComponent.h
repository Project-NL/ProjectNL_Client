// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerSpringArmComponent.generated.h"

/**
 * 
 */
UCLASS()
//플레이어 스프링암 컴포넌트
class PROJECTNL_API UPlayerSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UPlayerSpringArmComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
