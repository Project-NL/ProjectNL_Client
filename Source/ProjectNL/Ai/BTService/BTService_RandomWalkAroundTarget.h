// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_RandomWalkAroundTarget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNL_API UBTService_RandomWalkAroundTarget : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_RandomWalkAroundTarget();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	
	/** 이동 반경 */
	UPROPERTY(EditAnywhere, Category = "Movement")
	float WalkRadius = 500.0f;
};