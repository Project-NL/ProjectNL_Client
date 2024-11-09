#pragma once
#include "AbilitySystemComponent.h"
#include "ProjectNL/Helper/EnumHelper.h"
#include "ProjectNL/Helper/GameplayTagHelper.h"
#include "ProjectNL/Weapon/BaseWeapon.h"

class FLocateHelper
{
public:
	FORCEINLINE static EMovementDirection GetDirectionByVector(const FVector2D Vector)
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
};
