#pragma once

#include "CoreMinimal.h"

enum class EMovementDirection : uint8;

class PROJECTNL_API FLocateHelper
{
public:
	static EMovementDirection GetDirectionByVector(const FVector2D Vector);

	static FRotator GetTargetingRotate(const AActor* OwnerActor, const AActor* TargetActor);
};
