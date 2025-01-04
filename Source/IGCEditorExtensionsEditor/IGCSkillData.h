// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IGCSkillData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSkillStepData
{
	GENERATED_BODY()

public:
	// 이 단계의 시작 / 끝 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timeline")
	float StartTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timeline")
	float EndTime;

	// 어떤 동작인지 구분할 용도 (애니메이션 / 이동 / 파티클 등)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timeline")
	FString ActionType; 

	// 예시: 파티클 에셋, 애니메이션 에셋 등 레퍼런스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timeline")
	TSoftObjectPtr<UAnimationAsset> AnimationAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timeline")
	TSoftObjectPtr<UParticleSystem> ParticleAsset;
    
	// 이동오프셋 등 필요 속성...
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timeline")
	FVector MoveOffset;
};
UCLASS()
class IGCEDITOREXTENSIONSEDITOR_API UIGCSkillData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Basic")
	FString SkillName;

	// 여러 개의 스텝(키프레임)들
	UPROPERTY(EditAnywhere, Category="Timeline")
	TArray<FSkillStepData> Steps;
	
};
