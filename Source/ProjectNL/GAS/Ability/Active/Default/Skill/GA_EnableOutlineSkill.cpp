// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectNL/GAS/Ability/Active/Default/Skill/GA_EnableOutlineSkill.h"

#include "Kismet/GameplayStatics.h"

UGA_EnableOutlineSkill::UGA_EnableOutlineSkill(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer), bEnableLine{}
{

}

void UGA_EnableOutlineSkill::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	OutlineMaterialInstanceDynamic = UMaterialInstanceDynamic::Create(OutlineMaterialInstance, GetWorld());
	APostProcessVolume* Volume = Cast<APostProcessVolume>(UGameplayStatics::GetActorOfClass(GetWorld(), APostProcessVolume::StaticClass()));
	Volume->Settings.WeightedBlendables.Array.Add(FWeightedBlendable(1.f, OutlineMaterialInstanceDynamic));
	// 값 반전: 0 -> 1, 1 -> 0
	bEnableLine= (bEnableLine == 0.0f) ? 1.0f : 0.0f;
	OutlineMaterialInstanceDynamic->SetScalarParameterValue(FName("Enable"), bEnableLine);

	
	/*스킬 애니메이션 추가
	 *
	 *
	*/
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);


}

void UGA_EnableOutlineSkill::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_EnableOutlineSkill::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
}

void UGA_EnableOutlineSkill::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
}

void UGA_EnableOutlineSkill::PlayActionAnimation()
{
}
