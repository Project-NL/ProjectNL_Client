// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GhostTrailPoolManager.generated.h"

class AGhostTrail;
UCLASS()
class PROJECTNL_API AGhostTrailPoolManager : public AActor
{
	GENERATED_BODY()

public:
	AGhostTrailPoolManager();

	// 풀에서 GhostTrail 하나를 가져오는 함수
	UFUNCTION(BlueprintCallable, Category="GhostTrailPooling")
	AGhostTrail* GetPooledGhostTrail();

	// 사용이 끝난 GhostTrail을 풀에 반환하는 함수
	UFUNCTION(BlueprintCallable, Category="GhostTrailPooling")
	void ReturnGhostTrailToPool(AGhostTrail* GhostTrail);

	// 풀 초기화 함수
	void InitializePool(UWorld* World, TSubclassOf<AGhostTrail> GhostTrailClass, int32 InitialSize);

protected:
	UPROPERTY(EditAnywhere, Category="GhostTrailPooling")
	int32 InitialPoolSize;

	UPROPERTY()
	TArray<AGhostTrail*> GhostTrailPool;

	UPROPERTY()
	TSubclassOf<AGhostTrail> GhostTrailActorClass;
};