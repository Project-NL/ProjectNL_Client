// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectNL/Ai/BTDecorator/BTDecorator_CheckGamePlayTag.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemComponent.h"

UBTDecorator_CheckGamePlayTag::UBTDecorator_CheckGamePlayTag()
{
	NodeName = "Check Gameplay Tags"; // BehaviorTree에서 보여질 이름
}

bool UBTDecorator_CheckGamePlayTag::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* OwnerController = Cast<AAIController>(OwnerComp.GetOwner());
	APawn* ControlledPawn = OwnerController ? OwnerController->GetPawn() : nullptr;

	// PerformConditionCheckAI를 호출해 조건 검사
	return PerformConditionCheckAI(OwnerController, ControlledPawn);
}

bool UBTDecorator_CheckGamePlayTag::PerformConditionCheckAI(AAIController* OwnerController, APawn* ControlledPawn) const
{
	if (!OwnerController || !ControlledPawn)
	{
		return false;
	}

	// Blackboard 컴포넌트 가져오기
	UBlackboardComponent* BlackboardComp = OwnerController->GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return false;
	}

	// Blackboard에서 타겟 액터 가져오기
	UObject* TargetObject = BlackboardComp->GetValueAsObject(TargetBlackboardKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(TargetObject);
	if (!TargetActor)
	{
		return false;
	}

	// Ability System Component(ASC) 가져오기
	UAbilitySystemComponent* ASC = TargetActor->FindComponentByClass<UAbilitySystemComponent>();
	if (!ASC)
	{
		return false;
	}

	// ASC에서 태그 가져오기
	FGameplayTagContainer OwnedTags;
	ASC->GetOwnedGameplayTags(OwnedTags);

	// 태그 조건 확인
	if (HasAnyTags)
	{
		return OwnedTags.HasAny(GameplayTagContainer);
	}
	else
	{
		return OwnedTags.HasAll(GameplayTagContainer);
	}
}