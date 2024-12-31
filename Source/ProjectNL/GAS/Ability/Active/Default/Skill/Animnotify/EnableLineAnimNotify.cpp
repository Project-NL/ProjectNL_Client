// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectNL/GAS/Ability/Active/Default/Skill/Animnotify/EnableLineAnimNotify.h"

#include "Kismet/GameplayStatics.h"

void UEnableLineAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                   const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp || !OutlineMaterialInstance)
	{
		return;
	}

	// Safest way: get the owner of the MeshComp -> that actor has GetWorld().
	OwnerActor = MeshComp->GetOwner();
	if (!OwnerActor)
	{
		return;
	}

	 World = OwnerActor->GetWorld();
	if (!World)
	{
		return;
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,                               // Key (-1: 계속 새 메시지 생성)
			5.f,                              // 표시 시간(초)
			FColor::Green,                    // 글자 색
			TEXT("EnableLineAnimNotify::Notify 호출됨!") // 표시할 텍스트
		);
	}
	// Now we have a valid World context, continue
	OutlineMaterialInstanceDynamic = UMaterialInstanceDynamic::Create(OutlineMaterialInstance, World);
	APostProcessVolume* Volume = Cast<APostProcessVolume>(
		UGameplayStatics::GetActorOfClass(World, APostProcessVolume::StaticClass())
	);

	if (Volume)
	{
		Volume->Settings.WeightedBlendables.Array.Add(
			FWeightedBlendable(1.f, OutlineMaterialInstanceDynamic)
		);
	}
	bEnableLine= true;
	OutlineMaterialInstanceDynamic->SetScalarParameterValue(FName("Enable"), bEnableLine);
	World->GetTimerManager().SetTimer(
		OutlineDisableTimerHandle,   // FTimerHandle
		this,                        // 대상(멤버함수 호출용)
		&UEnableLineAnimNotify::DisableOutline, // 5초 후 호출할 함수
		5.0f,                        // 지연 시간(5초)
		false                        // 반복 여부 (false: 1회만 실행)
	);
	
}

void UEnableLineAnimNotify::DisableOutline()
{
	// bEnableLine을 false로 전환
	bEnableLine = false;
	if (OutlineMaterialInstanceDynamic)
	{
		APostProcessVolume* Volume = Cast<APostProcessVolume>(
		UGameplayStatics::GetActorOfClass(World, APostProcessVolume::StaticClass())
	);

		if (Volume)
		{
			Volume->Settings.WeightedBlendables.Array.Add(
				FWeightedBlendable(1.f, OutlineMaterialInstanceDynamic)
			);
		}
		OutlineMaterialInstanceDynamic->SetScalarParameterValue(FName("Enable"),float(bEnableLine));

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				5.f,
				FColor::Green,
				TEXT("EnableLineAnimNotify: DisableOutline() called!")
			);
		}
	}

	

	

}