#include "GA_Guard.h"

#include "AnimNotify/BlockStartNotify.h"
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
		// EquipComponent가 다 실행되기 이전에 해당 Ability가 실행되는 경우는
		// 이 액션이 중단될 수 있으니 주의하면 좋다.
		// TODO: 애니메이션 Binding에 대해 AvatarSet에서 진행해 EquipComponent에 주입하는 것도 좋아보임
		SetCurrentMontage(EquipComponent->GetBlockAnim());
		for (FAnimNotifyEvent NotifyEvent : GetCurrentMontage()->Notifies)
		{
			if (UBlockStartNotify* BSNotify = Cast<UBlockStartNotify>(NotifyEvent.Notify))
         	{
         		BSNotify->OnNotified.AddDynamic(this, &ThisClass::OnBlockStart);
				BlockStartNotify = BSNotify;
         	}
		}
		
		AnimationTask = UPlayMontageWithEvent::InitialEvent(this, NAME_None,
			GetCurrentMontage(), FGameplayTagContainer());
		AnimationTask->OnCancelled.AddDynamic(this, &ThisClass::UGA_Guard::EndBlock);
		AnimationTask->OnCompleted.AddDynamic(this, &ThisClass::UGA_Guard::EndBlock);
	}
}

void UGA_Guard::OnBlockStart()
{
	FStateHelper::ChangePlayerState(GetAbilitySystemComponentFromActorInfo(), NlGameplayTags::Status_Block, NlGameplayTags::Status_Guard);
}


void UGA_Guard::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	GetAbilitySystemComponentFromActorInfo()->SetLooseGameplayTagCount(NlGameplayTags::Status_Block, 0);
	GetAbilitySystemComponentFromActorInfo()->SetLooseGameplayTagCount(NlGameplayTags::Status_Guard, 0);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGA_Guard::EndBlock(FGameplayTag EventTag, FGameplayEventData EventData)
{
	check(BlockStartNotify)
	BlockStartNotify->OnNotified.RemoveAll(this);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}


void UGA_Guard::GuardDamage(float Damage)
{
	// 가드 상태일 때 실행하는 함수
	if (AnimationTask && GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(NlGameplayTags::Status_Guard))
	{
		FStateHelper::ChangePlayerState(GetAbilitySystemComponentFromActorInfo(), NlGameplayTags::Status_Guard, NlGameplayTags::Status_Block);
		AnimationTask->ReadyForActivation();
		return;
	}

	// 그 외에 즉 Blocking 상태인 경우 실행하는 함수
	if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(NlGameplayTags::Status_Block))
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(NlGameplayTags::Status_Block);
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}
