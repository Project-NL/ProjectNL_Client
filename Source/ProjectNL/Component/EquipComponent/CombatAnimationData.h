#pragma once

#include "CoreMinimal.h"
#include "CombatAnimationData.generated.h"

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
