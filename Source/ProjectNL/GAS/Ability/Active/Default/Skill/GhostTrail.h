// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GhostTrail.generated.h"


class UPoseableMeshComponent;
class UMaterialInstance;
class UMaterialInstanceDynamic;
class USkeletalMeshComponent;

UCLASS()
class PROJECTNL_API AGhostTrail : public AActor
{
	GENERATED_BODY()
	
public:	
	AGhostTrail();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="GhostTrail")
	void Init(USkeletalMeshComponent* Pawn);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GhostTrail")
	float FadeOutTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GhostTrail")
	UMaterialInstance* GhostMaterial;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UPoseableMeshComponent* PoseableMesh;

	bool IsSpawned = false;
	float FadeCountDown = 0.0f;

	// Tick에서 사용할 Dynamic Material Instance 참조
	UMaterialInstanceDynamic* GhostMaterialInstance = nullptr;

};
