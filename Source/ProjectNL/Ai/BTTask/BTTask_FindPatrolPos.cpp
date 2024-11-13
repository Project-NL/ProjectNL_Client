// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectNL/Ai/BTTask/BTTask_FindPatrolPos.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectNL/Ai/AiKey.h"


UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
}
EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	APawn* ControllPawn=OwnerComp.GetAIOwner()->GetPawn();
	if(nullptr==ControllPawn)
	{
		return  EBTNodeResult::Failed;
	}
	UNavigationSystemV1 * NavSystem= UNavigationSystemV1::GetNavigationSystem(ControllPawn->GetWorld());
	if(nullptr==NavSystem)
	{
		return  EBTNodeResult::Failed;
	}
	float PatrolRadius =300;
	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_HOMEPOS);
	FNavLocation NextPatrolPos;

	if(NavSystem->GetRandomPointInNavigableRadius(Origin,PatrolRadius,NextPatrolPos))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_PATROLPOS,NextPatrolPos.Location);
		return EBTNodeResult::Succeeded;
	}
		
	
	return EBTNodeResult::Failed;
}
