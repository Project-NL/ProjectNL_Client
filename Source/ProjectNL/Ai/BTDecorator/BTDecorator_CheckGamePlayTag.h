// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckGamePlayTag.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNL_API UBTDecorator_CheckGamePlayTag : public UBTDecorator
{
	GENERATED_BODY()
public:
	// 생성자
	UBTDecorator_CheckGamePlayTag();
	/** Decorator의 조건을 검사하는 함수 */
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	bool PerformConditionCheckAI(AAIController* OwnerController, APawn* ControlledPawn) const;

private:
	/** Blackboard에서 사용할 키 (대상 액터) */
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetBlackboardKey;

	/** 검사할 Gameplay 태그 컨테이너 */
	UPROPERTY(EditAnywhere, Category = "GameplayTags")
	FGameplayTagContainer GameplayTagContainer;

	/** HasAnyTags: true면 하나라도 태그가 포함되는지 검사, false면 모든 태그 포함 여부 검사 */
	UPROPERTY(EditAnywhere, Category = "GameplayTags")
	bool HasAnyTags = true;
};
