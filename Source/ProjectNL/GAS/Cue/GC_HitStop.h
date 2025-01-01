// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "GC_HitStop.generated.h"

UCLASS()
class PROJECTNL_API AGC_HitStop : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()

public:
	AGC_HitStop();

protected:
	virtual bool OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
};
