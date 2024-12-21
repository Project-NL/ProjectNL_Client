// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectNL/GAS/Ability/Active/Default/Skill/GA_EnableOutlineSkill.h"

#include "Kismet/GameplayStatics.h"
#include "ProjectNL/GAS/Ability/Utility/PlayMontageWithEvent.h"
#include "ProjectNL/Helper/GameplayTagHelper.h"

UGA_EnableOutlineSkill::UGA_EnableOutlineSkill(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer), bEnableLine{}
{

}

void UGA_EnableOutlineSkill::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	


	
	/*스킬 애니메이션 추가
	
	 *
	*/
	//if (K2_HasAuthority())
	//{
		PlayActionAnimation();
	//}
	//EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	// GetWorld()->GetTimerManager().SetTimer(
	// OutlineDisableTimerHandle,   // FTimerHandle
	// this,                        // 대상(멤버함수 호출용)
	// &UGA_EnableOutlineSkill::DisableOutline, // 5초 후 호출할 함수
	// 5.0f,                        // 지연 시간(5초)
	// false );
}

void UGA_EnableOutlineSkill::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_EnableOutlineSkill::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true
		   , false);
}

void UGA_EnableOutlineSkill::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true
		   , false);
}

void UGA_EnableOutlineSkill::PlayActionAnimation()
{

	PlayMontageTask = UPlayMontageWithEvent::InitialEvent(this, NAME_None
																  , SkillAnimMontage
																  , FGameplayTagContainer());
	PlayMontageTask->OnCancelled.AddDynamic(this, &ThisClass::OnCancelled);
	PlayMontageTask->OnBlendOut.AddDynamic(this, &ThisClass::OnCompleted);
	PlayMontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnCompleted);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnCompleted);

	PlayMontageTask->ReadyForActivation();
}


void UGA_EnableOutlineSkill::DisableOutline()
{
	// bEnableLine을 false로 전환
	bEnableLine = false;
	OutlineMaterialInstanceDynamic = UMaterialInstanceDynamic::Create(OutlineMaterialInstance, GetWorld());
	APostProcessVolume* Volume = Cast<APostProcessVolume>(
		UGameplayStatics::GetActorOfClass(GetWorld(), APostProcessVolume::StaticClass())
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

