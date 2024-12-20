#pragma once

#include "CoreMinimal.h"
#include "ProjectNL/Helper/EnumHelper.h"
#include "CombatAnimationData.generated.h"

enum class EMovementDirection : uint8;
enum class ETargetHeight : uint8;

USTRUCT(BlueprintType)
struct FAnimationBy4RotationWithHeight
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> F_High_Animation;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> L_High_Animation;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> R_High_Animation;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> B_High_Animation;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> F_Middle_Animation;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> L_Middle_Animation;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> R_Middle_Animation;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> B_Middle_Animation;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> F_Low_Animation;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> L_Low_Animation;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> R_Low_Animation;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> B_Low_Animation;

	// TODO: 해당 함수는 매번 구조체 활성화 및 호출 시 호출되는 함수이기 때문에
	// 메모리 오버헤드가 발생할 수 있으나 우선 코드 안정성을 위해 여기에 보관함.
	TObjectPtr<UAnimMontage> GetAnimationByDirection(const EMovementDirection Direction, const ETargetHeight Height)
	{
		if (Height == ETargetHeight::High)
		{
			if (Direction == EMovementDirection::F) return F_High_Animation;
			if (Direction == EMovementDirection::B) return B_High_Animation;
			if (Direction == EMovementDirection::L) return L_High_Animation;
			if (Direction == EMovementDirection::R) return R_High_Animation;
		}
		if (Height == ETargetHeight::Middle)
		{
			if (Direction == EMovementDirection::F) return F_Middle_Animation;
			if (Direction == EMovementDirection::B) return B_Middle_Animation;
			if (Direction == EMovementDirection::L) return L_Middle_Animation;
			if (Direction == EMovementDirection::R) return R_Middle_Animation;
		}
		if (Height == ETargetHeight::Low)
		{
			if (Direction == EMovementDirection::F) return F_Low_Animation;
			if (Direction == EMovementDirection::B) return B_Low_Animation;
			if (Direction == EMovementDirection::L) return L_Low_Animation;
			if (Direction == EMovementDirection::R) return R_Low_Animation;
		}
		return nullptr;
	}
};

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
	TArray<TObjectPtr<UAnimMontage>> ComboAttackAnim;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> HeavyAttackAnim;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> JumpAttackAnim;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> EquipAnim;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> UnEquipAnim;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> BlockAnim;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FAnimationByRotation EvadeAnim;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FAnimationByRotation StepAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FAnimationBy4RotationWithHeight DamagedAnim;
};
