// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectNL/Ai/BTService/BTService_RandomWalkAroundTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectNL/Helper/AbilityHelper.h"

#include "Navigation/PathFollowingComponent.h"
#include "ProjectNL/Ai/AiKey.h"
#include "ProjectNL/Character/BaseCharacter.h"
#include "ProjectNL/Character/Enemy/EnemyCharacter.h"
#include "ProjectNL/GAS/Attribute/BaseAttributeSet.h"

UBTService_RandomWalkAroundTarget::UBTService_RandomWalkAroundTarget()
{
	NodeName = TEXT("Random Walk Around Target");

	// 기본 호출 주기 설정
	bNotifyCeaseRelevant = true;
	Interval = 1.0f; // 1초마다 실행
	RandomDeviation = 0.2f; // 0.2초의 무작위 간격 추가
}

void UBTService_RandomWalkAroundTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return;
	}

	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn)
	{
		return;
	}

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return;
	}

	// 타겟 위치 가져오기
	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(BBKEY_NEAREST_ENEMY));
	if (!TargetActor)
	{
		return; // 타겟이 없으면 아무 것도 하지 않음
	}

	FVector TargetLocation = TargetActor->GetActorLocation();

	// 타겟 주변 랜덤 위치 계산
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSystem)
	{
		return;
	}

	FNavLocation RandomLocation;
	bool bFoundLocation = NavSystem->GetRandomReachablePointInRadius(TargetLocation, WalkRadius, RandomLocation);

	
	ControlledPawn->bUseControllerRotationYaw = false;

	// 이동 명령
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(RandomLocation);
	MoveRequest.SetAcceptanceRadius(50.0f);
	// 캐릭터 이동 컴포넌트 가져오기
	UCharacterMovementComponent* MovementComponent = ControlledPawn->FindComponentByClass<UCharacterMovementComponent>();
	if (MovementComponent)
	{
		MovementComponent->MaxWalkSpeed = 200;
	}
	FNavPathSharedPtr NavPath;
	AIController->MoveTo(MoveRequest, &NavPath);

}

void UBTService_RandomWalkAroundTarget::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return;
	}

	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn)
	{
		return;
	}
	ControlledPawn->bUseControllerRotationYaw = true;
	
	float BaseSpeed=Cast<UBaseAttributeSet>(AbilityHelper::GetAttribute(Cast<AEnemyCharacter>(ControlledPawn)))->GetMovementSpeed();
	Cast<ACharacter>(ControlledPawn)->GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
	
	
}
