#include "GA_ToggleWeapon.h"

#include "AnimNotify/GrabWeaponNotify.h"
#include "AnimNotify/PutWeaponNotify.h"
#include "ProjectNL/Component/EquipComponent/EquipComponent.h"
#include "ProjectNL/GAS/Ability/Utility/PlayMontageWithEvent.h"
#include "ProjectNL/Helper/StateHelper.h"

UGA_ToggleWeapon::UGA_ToggleWeapon(
	const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool UGA_ToggleWeapon::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle
	, const FGameplayAbilityActorInfo* ActorInfo
	, const FGameplayTagContainer* SourceTags
	, const FGameplayTagContainer* TargetTags
	, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags
																, OptionalRelevantTags))
	{
		return false;
	}
	if (const ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(
		ActorInfo->AvatarActor.Get()))
	{
		if (IsValid(OwnerCharacter->GetAbilitySystemComponent()))
		{
			return FStateHelper::IsPlayerIdle(
				OwnerCharacter->GetAbilitySystemComponent());
		}
	}
	return false;
}

void UGA_ToggleWeapon::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle
	, const FGameplayAbilityActorInfo* ActorInfo
	, const FGameplayAbilityActivationInfo ActivationInfo
	, const FGameplayEventData* TriggerEventData)
{
	if (ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(
		ActorInfo->AvatarActor.Get()))
	{
		const bool IsCombat = ToggleCombatState();
		SetCurrentMontage(IsCombat ?
			OwnerCharacter->GetEquipComponent()->GetEquipAnim() :
			OwnerCharacter->GetEquipComponent()->GetUnEquipAnim());

		AnimationTask = UPlayMontageWithEvent::InitialEvent(this,
			NAME_None, GetCurrentMontage(), FGameplayTagContainer());
		AnimationTask->OnCompleted.AddDynamic(this, &ThisClass::EndToggleWeapon);
		AnimationTask->ReadyForActivation();
	}
}

bool UGA_ToggleWeapon::ToggleCombatState()
{
	const bool IsCombat = GetAbilitySystemComponentFromActorInfo()
							->HasMatchingGameplayTag(NlGameplayTags::Status_Combat);
	
	FStateHelper::ChangePlayerState(GetAbilitySystemComponentFromActorInfo()
																	, NlGameplayTags::State_Idle
																	, IsCombat
																			? NlGameplayTags::State_PutWeapon
																			: NlGameplayTags::State_GrabWeapon, true);

	// 기존 상태에서 변화가 있었기 때문에 IsCombat 변수의 부정 값을 반환
	return !IsCombat;
}

void UGA_ToggleWeapon::EndToggleWeapon(FGameplayTag EventTag, FGameplayEventData EventData)
{
	const bool IsCombat = GetAbilitySystemComponentFromActorInfo()
							->HasMatchingGameplayTag(NlGameplayTags::Status_Combat);
	if (IsCombat)
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(NlGameplayTags::Status_Combat);
		GetAbilitySystemComponentFromActorInfo()->RemoveReplicatedLooseGameplayTag(NlGameplayTags::Status_Combat);
		FStateHelper::ChangePlayerState(GetAbilitySystemComponentFromActorInfo()
																	, NlGameplayTags::State_PutWeapon
																	,  NlGameplayTags::State_Idle, true);
	} else
	{
		GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(NlGameplayTags::Status_Combat);
		GetAbilitySystemComponentFromActorInfo()->AddReplicatedLooseGameplayTag(NlGameplayTags::Status_Combat);
		FStateHelper::ChangePlayerState(GetAbilitySystemComponentFromActorInfo()
																	, NlGameplayTags::State_GrabWeapon
																	, NlGameplayTags::State_Idle, true);
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}


