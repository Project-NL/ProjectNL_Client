// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimeRecallComponent.generated.h"


class AGhostTrail;
class AGhostTrailPoolManager;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTNL_API UTimeRecallComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTimeRecallComponent();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; // 추가
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	/** Transform을 기록할 주기(초 단위) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TimeRecall")
	float RecordInterval;

	/** 최대 기록 시간(초 단위). 이 시간을 초과하는 기록은 오래된 순서대로 제거됨 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TimeRecall")
	float MaxRecordTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TimeRecall")
	TSubclassOf<AGhostTrail> GhostTrailActor;
	/** 기록된 Transform을 역재생하기(시간 역행) 시작하는 함수 */
	UFUNCTION()
	void StartRecall(float RecallDuration);

	/** Recall 중 Transform을 되돌리는 처리 */
	UFUNCTION()
	void PerformRecall(float DeltaTime);



private:

	/** Transform 기록 배열 */
	UPROPERTY()
	TObjectPtr<AGhostTrailPoolManager> GhostTrailPoolManager;

	UPROPERTY()
	TArray<FTransform> StoredTransforms;

	UPROPERTY(EditAnywhere, Category="GhostTrailPooling")
	int32 InitialPoolSize;

	UPROPERTY()
	float TimeAccumulator;
	
	int32 MaxTransformsCount;
	UPROPERTY(REplicatedUsing = OnRep_bIsRecalling)
	int8 bIsRecalling ;
	UFUNCTION()
	void OnRep_BIsRecalling();
	float RecallTimeRemaining;
	
	float RecordTime;
	


};

