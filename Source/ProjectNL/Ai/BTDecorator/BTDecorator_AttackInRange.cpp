// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectNL/Ai/BTDecorator/BTDecorator_AttackInRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectNL/Ai/AiKey.h"

UBTDecorator_AttackInRange::UBTDecorator_AttackInRange()
{
}

bool UBTDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return false;
	}
	
	//ICharacterAiInterface* AIPawn = Cast<ICharacterAiInterface>(ControllingPawn);
	// if (nullptr == AIPawn)
	// {
	// 	return false;
	// }
	//
	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == Target)
	{
		return false;
	}
	
	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);
	float AttackRangeWithRadius = 300;//임의값 수정 필요
	bResult = (DistanceToTarget <= AttackRangeWithRadius);
	UE_LOG(LogTemp, Log, TEXT("AttackinRange result: %s"), bResult ? TEXT("true") : TEXT("false"));
	return bResult;
}
