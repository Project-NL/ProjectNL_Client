// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectNL/GAS/Ability/Active/Default/Skill/GhostTrail.h"

#include "Components/PoseableMeshComponent.h"
#include "Kismet/KismetMaterialLibrary.h"

// Sets default values
AGhostTrail::AGhostTrail()
{
	PrimaryActorTick.bCanEverTick = true;
 //USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SCENEROOT"));
 //   RootComponent = SceneRoot;

    PoseableMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("POSEABLEMESH"));
	
    SetRootComponent(PoseableMesh);//PoseableMesh->SetupAttachment(RootComponent);
	
	
}

void AGhostTrail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsSpawned && GhostMaterialInstance)
	{
		FadeCountDown -= DeltaTime;
		float Opacity = FMath::Clamp(FadeCountDown / FadeOutTime, 0.0f, 1.0f);

		// 동적 머티리얼 파라미터 업데이트
		GhostMaterialInstance->SetScalarParameterValue("Opacity", Opacity);

		if (FadeCountDown <= 0.0f)
		{
			SetActorHiddenInGame(true);
			SetActorEnableCollision(false);
		}
	}
}

void AGhostTrail::Init(USkeletalMeshComponent* Pawn)
{
	if (!PoseableMesh || !Pawn)
	{
		return;
	}

	// Pawn으로부터 Pose 복사
	PoseableMesh->CopyPoseFromSkeletalComponent(Pawn);

	// GhostMaterial로부터 Dynamic Material Instance 생성
	if (GhostMaterial)
	{
		GhostMaterialInstance = UMaterialInstanceDynamic::Create(GhostMaterial, this);
		GhostMaterialInstance->SetScalarParameterValue("Opacity", 0);
		if (GhostMaterialInstance)
		{
			// PoseableMesh의 모든 Material Slot에 적용
			int32 MaterialCount = Pawn->GetNumMaterials();
			for (int32 i = 0; i < MaterialCount; ++i)
			{
				PoseableMesh->SetMaterial(i, GhostMaterialInstance);
			}
		}
	}

	// 페이드 카운트다운 초기화
	FadeCountDown = FadeOutTime;
	IsSpawned = true;
}