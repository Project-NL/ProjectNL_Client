// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectNL/Ai/BTTask/BTTask_DirectTo.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_DirectTo::UBTTask_DirectTo()
{
}

EBTNodeResult::Type UBTTask_DirectTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	// Get the AI Controller
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	// Get the controlled pawn
	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}

	// Get the target actor from the blackboard
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	UObject* TargetObject = BlackboardComp->GetValueAsObject("Target"); // Replace "TargetActor" with your actual blackboard key
	AActor* TargetActor = Cast<AActor>(TargetObject);
	if (!TargetActor)
	{
		return EBTNodeResult::Failed;
	}

	// Calculate the direction to the target
	FVector TargetLocation = TargetActor->GetActorLocation();
	FVector PawnLocation = ControlledPawn->GetActorLocation();

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(PawnLocation, TargetLocation);

	// Set the actor's rotation to face the target
	ControlledPawn->SetActorRotation(LookAtRotation);

	return EBTNodeResult::Succeeded;
}
