#include "GC_HitStop.h"

#include "Components/TimelineComponent.h"
#include "Engine/OverlapResult.h"


AGC_HitStop::AGC_HitStop()
{
	// GC 관련 액터가 보일 필요는 없다.
	SetHidden(true);
	HitStopTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("HitStopTimeline"));
	HitStopCallback.BindUFunction(this, "HitStopPlayCallback");
}

void AGC_HitStop::HitStopPlayCallback(float Output)
{
	UE_LOG(LogTemp, Display, TEXT("Test Result Value: %f"), Output);
	TargetActor->CustomTimeDilation = Output;
	SourceActor->CustomTimeDilation = Output;

	for (FOverlapResult& OverlapResult : OverlapResults)
	{
		OverlapResult.GetActor()->CustomTimeDilation = Output;
	}
}


// 해당 액터 주변 N칸 이내의 모든 액터에게 적용할 필요가 있음
// 멀티 플레이 특성상 자신만 HitStop이 적용된다면 매우 번거로울 것
bool AGC_HitStop::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	if (Super::OnActive_Implementation(MyTarget, Parameters))
	{
		return true;
	}

	float SphereRadius = 300;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = false;
	QueryParams.bReturnPhysicalMaterial = false;

	TargetActor = MyTarget;
	SourceActor = Parameters.EffectCauser.Get();
	// 충돌 형상(구체) 정의
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(SphereRadius);

	MyTarget->GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		MyTarget->GetActorLocation(),
		FQuat::Identity,
		ECC_Visibility, // 보이는 모든 것들에 대한 처리가 필요하다.
		CollisionShape,
		QueryParams);

	HitStopTimeline->SetLooping(false);
	HitStopTimeline->AddInterpFloat(HitStopTimingCurve, HitStopCallback);
	HitStopTimeline->Play();
	
	return false;
}
