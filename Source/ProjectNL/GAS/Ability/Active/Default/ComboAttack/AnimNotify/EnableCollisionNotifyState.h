// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "EnableCollisionNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNL_API UEnableCollisionNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
protected:
	void PerformTriangleTrace(AActor* Owner, FVector Point1, FVector Point2, FVector Point3, TArray<FHitResult>& OutHitResults);

	void MakeTriangleTrace(AActor* Character);
private:
	

	UPROPERTY()
	TSet<AActor*> HitActors;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> AttackDamageEffect;
	// 이전 프레임의 소켓 위치를 저장하는 벡터
	FVector PrevStartLocation;
	FVector PrevEndLocation;

	
};
