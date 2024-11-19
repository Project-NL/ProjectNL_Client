#pragma once

#include "LocateHelper.h"

#include "Kismet/KismetMathLibrary.h"
#include "ProjectNL/Helper/EnumHelper.h"

EMovementDirection FLocateHelper::GetDirectionByVector(const FVector2D Vector)
{
	if (Vector.X == -1 && Vector.Y == 1) return EMovementDirection::FL;
	if (Vector.X == 1 && Vector.Y == 1) return EMovementDirection::FR;
	if (Vector.X == -1 && Vector.Y == 0) return EMovementDirection::L;
	if (Vector.X == 1 && Vector.Y == 0) return EMovementDirection::R;
	if (Vector.X == -1 && Vector.Y == -1) return EMovementDirection::BL;
	if (Vector.X == 1 && Vector.Y == -1) return EMovementDirection::BR;
	if (Vector.X == 0 && Vector.Y == -1) return EMovementDirection::B;
	return EMovementDirection::F;
}

// 액터간의 서로 방향에 대한 값을 반환한다.
FRotator FLocateHelper::GetTargetingRotate(const AActor* OwnerActor, const AActor* TargetActor)
{
	const FVector StartVector = OwnerActor->GetActorLocation();
	const FVector TargetVector = TargetActor->GetActorLocation();

	return UKismetMathLibrary::FindLookAtRotation(
		StartVector, TargetVector);
}
