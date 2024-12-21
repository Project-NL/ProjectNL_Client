// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EnableLineAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNL_API UEnableLineAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	/** This should be visible in the anim notify details now */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Material")
	UMaterialInstance* OutlineMaterialInstance;
private:
	
	UPROPERTY()
	UMaterialInstanceDynamic* OutlineMaterialInstanceDynamic;
	UPROPERTY()
	int8 bEnableLine{};

	/** Outline을 5초 후 끄는 함수를 타이머가 호출할 예정 */
	void DisableOutline();

	/** 타이머 핸들 */
	FTimerHandle OutlineDisableTimerHandle;

	AActor* OwnerActor;

	UWorld* World;
};
