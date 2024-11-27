// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_DirectTo.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNL_API UBTTask_DirectTo : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_DirectTo();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
};
