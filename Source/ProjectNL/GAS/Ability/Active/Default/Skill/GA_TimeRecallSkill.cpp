// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectNL/GAS/Ability/Active/Default/Skill/GA_TimeRecallSkill.h"

#include "GhostTrail.h"
#include "ProjectNL/Component/SkillComponent/TimeRecallComponent.h"

UGA_TimeRecallSkill::UGA_TimeRecallSkill(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

void UGA_TimeRecallSkill::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	ABaseCharacter* CharacterInfo = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
	check(CharacterInfo);
	CharacterInfo->GetTimeRecallComponent()->StartRecall(5);
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}

void UGA_TimeRecallSkill::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_TimeRecallSkill::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}



void UGA_TimeRecallSkill::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
}

void UGA_TimeRecallSkill::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
}

void UGA_TimeRecallSkill::PlayActionAnimation()
{
	
}

void UGA_TimeRecallSkill::PerformRecall(float DeltaTime)
{
}