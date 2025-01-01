// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "Animation/AnimMontage.h" 
#include "Curves/CurveVector.h"
#include "ProjectNL/Helper/EnumHelper.h"
#include "ActionAbilityData.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FActionSequenceData
{
	GENERATED_BODY()


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> ArrActionAnim; // 어떤 액션을 진행할 것인지

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EMovementDirection ArrMovementDirection; // 이동 방향

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UCurveVector> CurveVector;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DelayTime; // 딜레이 시간

};

USTRUCT(BlueprintType)
struct FActionAbilityData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FActionSequenceData> ArrActionSequenceDatas;
};