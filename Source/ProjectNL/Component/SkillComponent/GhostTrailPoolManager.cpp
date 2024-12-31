// Fill out your copyright notice in the Description page of Project Settings.


#include "projectNL/Component/SkillComponent/GhostTrailPoolManager.h"

#include "ProjectNL/GAS/Ability/Active/Default/Skill/GhostTrail.h"

AGhostTrailPoolManager::AGhostTrailPoolManager()
{
	PrimaryActorTick.bCanEverTick = false;
	InitialPoolSize = 10; // 초기 풀 크기
}

void AGhostTrailPoolManager::InitializePool(UWorld* World, TSubclassOf<AGhostTrail> GhostTrailClass, int32 InitialSize)
{
	GhostTrailActorClass = GhostTrailClass;
	InitialPoolSize = InitialSize;

	for (int32 i = 0; i < InitialPoolSize; i++)
	{
		AGhostTrail* NewGhostTrail = World->SpawnActor<AGhostTrail>(GhostTrailClass, FVector::ZeroVector, FRotator::ZeroRotator);
		if (NewGhostTrail)
		{
			NewGhostTrail->SetActorEnableCollision(false);
			NewGhostTrail->SetActorHiddenInGame(true);
			// 필요하다면 GhostTrail에 비활성화 상태 전환용 함수 호출
			GhostTrailPool.Add(NewGhostTrail);
		}
	}
}

AGhostTrail* AGhostTrailPoolManager::GetPooledGhostTrail()
{
	for (AGhostTrail* Trail : GhostTrailPool)
	{
		if (Trail && Trail->IsHidden()) // 간단히 Hidden 여부로 체크 (실제론 상태 변수 사용 권장)
		{
			Trail->SetActorHiddenInGame(false);
			Trail->SetActorEnableCollision(true);
			return Trail;
		}
	}

	// 풀에 여유가 없다면 새로운 GhostTrail 추가 생성(선택사항)
	// if (GhostTrailActorClass && GetWorld())
	// {
	// 	AGhostTrail* NewGhostTrail = GetWorld()->SpawnActor<AGhostTrail>(GhostTrailActorClass, FVector::ZeroVector, FRotator::ZeroRotator);
	// 	if (NewGhostTrail)
	// 	{
	// 		GhostTrailPool.Add(NewGhostTrail);
	// 		return NewGhostTrail;
	// 	}
	// }

	return nullptr;
}

void AGhostTrailPoolManager::ReturnGhostTrailToPool(AGhostTrail* GhostTrail)
{
	if (!GhostTrail) return;

	// GhostTrail 비활성화 처리
	GhostTrail->SetActorHiddenInGame(true);
	GhostTrail->SetActorEnableCollision(false);
	// 필요한 경우 Trail을 초기 상태로 되돌리는 로직 수행
}