#include "GA_Guard.h"

#include "ProjectNL/Character/Player/PlayerCharacter.h"
#include "ProjectNL/Component/EquipComponent/EquipComponent.h"
#include "ProjectNL/GAS/Ability/Utility/AT_PutDamageWithEvent.h"
#include "ProjectNL/GAS/Ability/Utility/PlayMontageWithEvent.h"
#include "ProjectNL/GAS/Attribute/PlayerAttributeSet.h"
#include "ProjectNL/Helper/GameplayTagHelper.h"
#include "ProjectNL/Helper/StateHelper.h"

UGA_Guard::UGA_Guard(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	
}

void UGA_Guard::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	GetAbilitySystemComponentFromActorInfo()->SetLooseGameplayTagCount(NlGameplayTags::Status_Guard, 1);

	DamageWithEventTask = UAT_PutDamageWithEvent::InitialEvent(this);
	DamageWithEventTask->OnDamage.AddDynamic(this, &ThisClass::GuardDamage);
	DamageWithEventTask->ReadyForActivation();

	
	if (ABaseCharacter* CharacterInfo = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo()))
	{
		UEquipComponent* EquipComponent = CharacterInfo->GetEquipComponent();
		check(EquipComponent)
		SetCurrentMontage(EquipComponent->GetBlockAnim());
		AnimationTask = UPlayMontageWithEvent::InitialEvent(this, NAME_None,
			GetCurrentMontage(), FGameplayTagContainer());
	}
}

void UGA_Guard::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	GetAbilitySystemComponentFromActorInfo()->SetLooseGameplayTagCount(NlGameplayTags::Status_Guard, 0);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGA_Guard::EndBlock(FGameplayTag EventTag, FGameplayEventData EventData)
{
	FStateHelper::ChangePlayerState(GetAbilitySystemComponentFromActorInfo(), NlGameplayTags::Status_Block, NlGameplayTags::Status_Guard);
}


void UGA_Guard::GuardDamage(float Damage)
{
	// 가드 상태일 때 실행하는 함수
	if (AnimationTask && GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(NlGameplayTags::Status_Guard))
	{
		FStateHelper::ChangePlayerState(GetAbilitySystemComponentFromActorInfo(), NlGameplayTags::Status_Guard, NlGameplayTags::Status_Block);
		AnimationTask->OnCancelled.AddDynamic(this, &ThisClass::UGA_Guard::EndBlock);
		AnimationTask->OnCompleted.AddDynamic(this, &ThisClass::UGA_Guard::EndBlock);
		AnimationTask->ReadyForActivation();
		return;
	}

	// 그 외에 즉 Blocking 상태인 경우 실행하는 함수
	if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(NlGameplayTags::Status_Block))
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(NlGameplayTags::Status_Block);
		check(AnimationTask)
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}
