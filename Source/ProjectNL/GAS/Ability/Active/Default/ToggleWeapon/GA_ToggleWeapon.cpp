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

		for (FAnimNotifyEvent NotifyEvent : GetCurrentMontage()->Notifies)
		{
			if (IsCombat)
			{
				if (UGrabWeaponNotify* GrabWeaponNotify = Cast<UGrabWeaponNotify>(NotifyEvent.Notify))
				{
					GrabWeaponNotify->OnNotified.AddDynamic(this, &ThisClass::GrabWeapon);
					ToggleWeaponNotify = GrabWeaponNotify;
				}
			} else
			{
				if (UPutWeaponNotify* PutWeaponNotify = Cast<UPutWeaponNotify>(NotifyEvent.Notify))
				{
					PutWeaponNotify->OnNotified.AddDynamic(this, &ThisClass::PutWeapon);
					ToggleWeaponNotify = PutWeaponNotify;
				}
			}

			// TODO: Get Main Weapon으로 가져와서 두 손 무기 인지 확인하고 처리하는 것이 더 편할 수 있다.
			// if (const TObjectPtr<USwapTwoHandWeaponNotify> SwapTwoHandNotify
			// 	= Cast<USwapTwoHandWeaponNotify>(NotifyEvent.Notify))
			// {
			// 	
			// }
		}

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
																			: NlGameplayTags::State_GrabWeapon);

	// 기존 상태에서 변화가 있었기 때문에 IsCombat 변수의 부정 값을 반환
	return !IsCombat;
}

void UGA_ToggleWeapon::GrabWeapon()
{
	if (ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(
		CurrentActorInfo->AvatarActor.Get()))
	{
		if (ABaseWeapon* MainWeapon = OwnerCharacter->GetEquipComponent()->GetMainWeapon())
		{
			MainWeapon->EquipCharacterWeapon(OwnerCharacter, true);
		}
		if (ABaseWeapon* SubWeapon = OwnerCharacter->GetEquipComponent()->GetSubWeapon())
		{
			SubWeapon->EquipCharacterWeapon(OwnerCharacter, false);
		}
	}
}

void UGA_ToggleWeapon::PutWeapon()
{
	if (ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(
		CurrentActorInfo->AvatarActor.Get()))
	{
		if (ABaseWeapon* MainWeapon = OwnerCharacter->GetEquipComponent()->GetMainWeapon())
		{
			MainWeapon->UnEquipCharacterWeapon(OwnerCharacter, true);
		}
		if (ABaseWeapon* SubWeapon = OwnerCharacter->GetEquipComponent()->GetSubWeapon())
		{
			SubWeapon->UnEquipCharacterWeapon(OwnerCharacter, false);
		}
	}
}

void UGA_ToggleWeapon::EndToggleWeapon(FGameplayTag EventTag, FGameplayEventData EventData)
{
	ClearDelegate();
	const bool IsCombat = GetAbilitySystemComponentFromActorInfo()
							->HasMatchingGameplayTag(NlGameplayTags::Status_Combat);
	if (IsCombat)
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(NlGameplayTags::Status_Combat);
		FStateHelper::ChangePlayerState(GetAbilitySystemComponentFromActorInfo()
																	, NlGameplayTags::State_PutWeapon
																	,  NlGameplayTags::State_Idle);
	} else
	{
		GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(NlGameplayTags::Status_Combat);
		FStateHelper::ChangePlayerState(GetAbilitySystemComponentFromActorInfo()
																	, NlGameplayTags::State_GrabWeapon
																	, NlGameplayTags::State_Idle);
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_ToggleWeapon::ClearDelegate()
{
	if (const TObjectPtr<UPutWeaponNotify> PutWeaponNotify = Cast<UPutWeaponNotify>(ToggleWeaponNotify))
	{
		PutWeaponNotify->OnNotified.RemoveAll(this);
	}
	if (const TObjectPtr<UGrabWeaponNotify> GrabWeaponNotify = Cast<UGrabWeaponNotify>(ToggleWeaponNotify))
	{
		GrabWeaponNotify->OnNotified.RemoveAll(this);
	}
}



