#include "ComboAttack.h"
#include "ProjectNL/GAS/Ability/Utility/PlayMontageWithEvent.h"
#include "ProjectNL/Component/EquipComponent/EquipComponent.h"
#include "ProjectNL/GAS/Ability/Active/Default/ComboAttack/AnimNotify/ComboAttackNotifyState.h"
#include "ProjectNL/Manager/AnimNotifyManager.h"

UComboAttack::UComboAttack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UComboAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle
																	, const FGameplayAbilityActorInfo* ActorInfo
																	, const FGameplayAbilityActivationInfo
																	ActivationInfo
																	, const FGameplayEventData* TriggerEventData)
{
	if (ABaseCharacter* CurrentCharacter = Cast<ABaseCharacter>(
		GetAvatarActorFromActorInfo()))
	{

		if (IsValid(Task))
		{
			Task->ExternalCancel();
		}

		const TArray<TObjectPtr<UAnimMontage>> ComboAttack = CurrentCharacter->
			GetEquipComponent()->GetComboAttackAnim();
		MaxCombo = ComboAttack.Num();
		if (!IsValid(ComboAttack[ComboIndex]))
		{
			UE_LOG(LogTemp, Error, TEXT("Fail to Load Combo Animation Try Again"));
			ComboIndex = 0;
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo
								, true, true);
		}

		SetCurrentMontage(ComboAttack[ComboIndex]);

		// TODO: 메인 서브 선택해서 Delegate 실행되게끔 처리
		ComboAttackNotifyState = UAnimNotifyManager::FindNotifyStateByClass<
			UComboAttackNotifyState>(GetCurrentMontage());

		if (IsValid(ComboAttackNotifyState))
		{
			ComboAttackNotifyState->OnNotifiedBegin.Clear();
			ComboAttackNotifyState->OnNotifiedBegin.AddDynamic(
				this, &UComboAttack::HandleComboNotifyStart);
			ComboAttackNotifyState->OnNotifiedEnd.Clear();
			ComboAttackNotifyState->OnNotifiedEnd.AddDynamic(
				this, &UComboAttack::HandleComboNotifyEnd);
		}

		Task = UPlayMontageWithEvent::InitialEvent(this, NAME_None
																							, GetCurrentMontage()
																							, FGameplayTagContainer());
		Task->OnCancelled.AddDynamic(this, &UComboAttack::OnCancelled);
		Task->OnInterrupted.AddDynamic(this, &UComboAttack::OnCancelled);
		Task->OnBlendOut.AddDynamic(this, &UComboAttack::OnCancelled);
		Task->OnCompleted.AddDynamic(this, &UComboAttack::OnCompleted);
		Task->ReadyForActivation();
	}
}

void UComboAttack::HandleComboNotifyStart(const EHandEquipStatus AttackHand)
{
	// TODO: 공격 시 line-trace 관련 코드 여기에 추가하면 좋다.
}

void UComboAttack::HandleComboNotifyEnd(const EHandEquipStatus AttackHand)
{
	ComboIndex = ComboIndex == MaxCombo - 1 ? 0 : ComboIndex + 1;
}

void UComboAttack::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	ComboIndex = 0;
	ClearDelegate();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true
						, false);
}

void UComboAttack::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	ClearDelegate();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true
						, false);
}

void UComboAttack::ClearDelegate()
{
	if (ComboAttackNotifyState)
	{
		ComboAttackNotifyState->OnNotifiedBegin.RemoveAll(this);
		ComboAttackNotifyState->OnNotifiedEnd.RemoveAll(this);
	}
}

void UComboAttack::EndAbility(const FGameplayAbilitySpecHandle Handle
															, const FGameplayAbilityActorInfo* ActorInfo
															, const FGameplayAbilityActivationInfo
															ActivationInfo, bool bReplicateEndAbility
															, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility
										, bWasCancelled);
}