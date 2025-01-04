#include "GC_HitStop.h"

#include "Components/TimelineComponent.h"
#include "Engine/OverlapResult.h"


AGC_HitStop::AGC_HitStop()
{
	SetHidden(true);
	HitStopTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("HitStop Timeline Component"));
}

void AGC_HitStop::BeginPlay()
{
	Super::BeginPlay();
	HitStopCallback.BindDynamic(this, &ThisClass::OnHitStopPlayCallback);
	HitStopFinish.BindDynamic(this, &ThisClass::OnHitStopFinish);
	
	HitStopTimeline->SetLooping(false);
	HitStopTimeline->AddInterpFloat(HitStopTimingCurve, HitStopCallback);
	HitStopTimeline->SetTimelineFinishedFunc(HitStopFinish);
}

void AGC_HitStop::OnHitStopPlayCallback(float Output)
{
	UE_LOG(LogTemp, Display, TEXT("HitStop 액션 진행"));
	TargetActor->CustomTimeDilation = Output;
	SourceActor->CustomTimeDilation = Output;

	// Target와 Source 에 대한 처리 이후 그 주변 사물의 속도 또한 조정할 필요가 있다.
	for (FOverlapResult& OverlapResult : OverlapResults)
	{
		OverlapResult.GetActor()->CustomTimeDilation = Output;
	}
}

void AGC_HitStop::OnHitStopFinish()
{
	UE_LOG(LogTemp, Display, TEXT("HitStop 액션 종료"));
	TargetActor->CustomTimeDilation = 1;
	SourceActor->CustomTimeDilation = 1;

	for (FOverlapResult& OverlapResult : OverlapResults)
	{
		OverlapResult.GetActor()->CustomTimeDilation = 1;
	}
}



// 해당 액터 주변 N칸 이내의 모든 액터에게 적용할 필요가 있음
// 멀티 플레이 특성상 자신만 HitStop이 적용된다면 매우 번거로울 것
bool AGC_HitStop::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	if (Super::OnExecute_Implementation(MyTarget, Parameters))
	{
		return true;
	}

	// TODO: 별도 변수 설정 가능하게 변경 처리
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = false;
	QueryParams.bReturnPhysicalMaterial = false;

	TargetActor = MyTarget;
	SourceActor = Parameters.EffectCauser.Get();
	// 충돌 형상(구체) 정의
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(HitStopRadius);

	MyTarget->GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		MyTarget->GetActorLocation(),
		FQuat::Identity,
		ECC_Visibility, // 보이는 모든 것들에 대한 처리가 필요하다.
		CollisionShape,
		QueryParams);
	
	HitStopTimeline->PlayFromStart();
	
	return false;
}
