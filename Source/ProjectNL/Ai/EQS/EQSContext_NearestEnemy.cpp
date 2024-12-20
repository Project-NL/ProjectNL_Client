// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectNL/Ai/EQS//EQSContext_NearestEnemy.h"

#include "AIController.h"
#include "ProjectNL/Ai/AiKey.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void UEQSContext_NearestEnemy::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);


	AActor* QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());
	if (!QueryOwner)
	{
		return;
	}


	AAIController* AIController = Cast<AAIController>(QueryOwner->GetInstigatorController());
	if (!AIController)
	{
		return;
	}


	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	if (!Blackboard)
	{
		return;
	}

	
	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(BBKEY_NEAREST_ENEMY));
	if (!TargetActor)
	{
		return;
	}
	
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, TargetActor);
}
