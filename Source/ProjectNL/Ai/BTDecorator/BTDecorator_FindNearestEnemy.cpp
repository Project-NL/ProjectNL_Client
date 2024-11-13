// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectNL/Ai/BTDecorator/BTDecorator_FindNearestEnemy.h"

UBTDecorator_FindNearestEnemy::UBTDecorator_FindNearestEnemy()
{
}

bool UBTDecorator_FindNearestEnemy::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	return Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
}
