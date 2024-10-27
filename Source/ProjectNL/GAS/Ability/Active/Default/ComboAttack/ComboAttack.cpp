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
		GetWorld()->GetTimerManager().ClearTimer(ComboClearTimerHandle);
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
		// TODO: 매번마다 Delegate를 연결하는 방식이 아닌, 최소한의 연결 방식을 활용하고 싶으나
		// 당장 나은 방식을 찾지는 못함. 별도의 트리거 변수를 사용한다면 가능할 수는 있으나
		// 코드가 난잡해지는 문제가 있을 수 있어 정확하게 효과적인지 비교는 해봐야함
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
		Task->OnBlendOut.AddDynamic(this, &UComboAttack::OnCompleted);
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
	ClearDelegate();
	GetWorld()->GetTimerManager().SetTimer(ComboClearTimerHandle, FTimerDelegate::CreateLambda([&]
	{
		ComboIndex = 0;
	}), ComboClearCooldown, false);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true
						, false);
}

void UComboAttack::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	ClearDelegate();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true
						, true);
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