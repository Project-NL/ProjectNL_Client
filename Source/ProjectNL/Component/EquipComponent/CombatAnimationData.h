#pragma once

#include "CoreMinimal.h"
#include "ProjectNL/Helper/EnumHelper.h"
#include "CombatAnimationData.generated.h"

enum class EMovementDirection : uint8;

USTRUCT(BlueprintType)
struct FAnimationByRotation
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> F_Animation;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> FL_Animation;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> FR_Animation;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> L_Animation;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> R_Animation;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> B_Animation;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> BL_Animation;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> BR_Animation;

	// TODO: 해당 함수는 매번 구조체 활성화 및 호출 시 호출되는 함수이기 때문에
	// 메모리 오버헤드가 발생할 수 있으나 우선 코드 안정성을 위해 여기에 보관함.
	TObjectPtr<UAnimMontage> GetAnimationByDirection(const EMovementDirection Direction)
	{
		if (Direction == EMovementDirection::F) return F_Animation;
		if (Direction == EMovementDirection::FR) return FR_Animation;
		if (Direction == EMovementDirection::FL) return FL_Animation;
		if (Direction == EMovementDirection::B) return B_Animation;
		if (Direction == EMovementDirection::BR) return BR_Animation;
		if (Direction == EMovementDirection::BL) return BL_Animation;
		if (Direction == EMovementDirection::L) return L_Animation;
		if (Direction == EMovementDirection::R) return R_Animation;
		return nullptr;
	}
};

USTRUCT(BlueprintType)
struct PROJECTNL_API FCombatAnimationData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<UAnimMontage*> AnimGroup;
};

USTRUCT(BlueprintType)
struct PROJECTNL_API FCombatAnimationByRotationData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FAnimationByRotation AnimGroup;
};
