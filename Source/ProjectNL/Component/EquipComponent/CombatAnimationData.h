#pragma once

#include "CoreMinimal.h"
#include "CombatAnimationData.generated.h"

USTRUCT(BlueprintType)
struct PROJECTNL_API FCombatAnimationData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<UAnimMontage*> AnimGroup;
};
