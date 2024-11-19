// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_FindNearestEnemy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNL_API UBTDecorator_FindNearestEnemy : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_FindNearestEnemy();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	UPROPERTY(EditAnywhere, Category = "Search")
	float SearchRadius;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector NearestEnemyKey;
	
};
