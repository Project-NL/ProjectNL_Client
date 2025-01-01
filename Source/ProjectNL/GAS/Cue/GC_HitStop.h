// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "Components/TimelineComponent.h"
#include "GC_HitStop.generated.h"

class UTimelineComponent;

UCLASS()
class PROJECTNL_API AGC_HitStop : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()

public:
	AGC_HitStop();

protected:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
	
private:
	UPROPERTY()
	TObjectPtr<UTimelineComponent> HitStopTimeline;
	
	UPROPERTY(EditAnywhere, Category="Options", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCurveFloat> HitStopTimingCurve;
	
	UPROPERTY()
	TObjectPtr<AActor> TargetActor;
	
	UPROPERTY()
	TObjectPtr<AActor> SourceActor;
	
	TArray<FOverlapResult> OverlapResults;
	
	FOnTimelineFloat HitStopCallback;

	void HitStopPlayCallback(float Output);
};
