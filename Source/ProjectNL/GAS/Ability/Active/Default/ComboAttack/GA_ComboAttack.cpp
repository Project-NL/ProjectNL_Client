#include "GA_ComboAttack.h"

#include "AnimNotify/ComboAttackEndNotify.h"
#include "ProjectNL/GAS/Ability/Utility/PlayMontageWithEvent.h"
#include "ProjectNL/Component/EquipComponent/EquipComponent.h"
#include "ProjectNL/GAS/Ability/Active/Default/ComboAttack/AnimNotify/ComboAttackNotifyState.h"

UGA_ComboAttack::UGA_ComboAttack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	ComboClearCooldown = 0;
}

bool UGA_ComboAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags
																, OptionalRelevantTags))
	{
		return false;
	}
	return true;
}


void UGA_ComboAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle
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

		for (FAnimNotifyEvent NotifyEvent : GetCurrentMontage()->Notifies)
		{
			if (const TObjectPtr<UComboAttackEndNotify> AttackEndNotify = Cast<UComboAttackEndNotify>(NotifyEvent.Notify))
			{
				ComboAttackEndNotify = AttackEndNotify;
				ComboAttackEndNotify->OnNotified.AddDynamic(this, &ThisClass::HandleComboEndNotify);
			}

			if (const TObjectPtr<UComboAttackNotifyState> AttackNotifyState = Cast<UComboAttackNotifyState>(NotifyEvent.NotifyStateClass))
			{
				ComboAttackNotifyState = AttackNotifyState;	
				ComboAttackNotifyState->OnNotifiedBegin.AddDynamic(
					this, &ThisClass::HandleComboNotifyStart);
				ComboAttackNotifyState->OnNotifiedEnd.AddDynamic(
					this, &ThisClass::HandleComboNotifyEnd);
			}
		}

		Task = UPlayMontageWithEvent::InitialEvent(this, NAME_None
																							, GetCurrentMontage()
																							, FGameplayTagContainer());
		Task->OnCancelled.AddDynamic(this, &UGA_ComboAttack::OnCancelled);
		Task->OnBlendOut.AddDynamic(this, &UGA_ComboAttack::OnCompleted);
		Task->OnCompleted.AddDynamic(this, &UGA_ComboAttack::OnCompleted);
		Task->ReadyForActivation();
	}
}

void UGA_ComboAttack::HandleComboNotifyStart(const EHandEquipStatus AttackHand)
{
	// TODO: 공격 시 line-trace 관련 코드 여기에 추가하면 좋다.
	UE_LOG(LogTemp, Display, TEXT("Test Combo Notify Start"))
}

void UGA_ComboAttack::HandleComboNotifyEnd(const EHandEquipStatus AttackHand)
{
	UE_LOG(LogTemp, Display, TEXT("Test Combo Notify End"))
	// TODO: 공격 line-trace 탐색 중단을 선언 하는 코드 추가
}

void UGA_ComboAttack::HandleComboEndNotify()
{
	ClearDelegate();
	ComboIndex = ComboIndex == MaxCombo - 1 ? 0 : ComboIndex + 1;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true
						, false);
}


void UGA_ComboAttack::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	ClearDelegate();
	GetWorld()->GetTimerManager().SetTimer(ComboClearTimerHandle, FTimerDelegate::CreateLambda([&]
	{
		ComboIndex = 0;
	}), ComboClearCooldown, false);
}

void UGA_ComboAttack::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	ClearDelegate();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true
						, true);
}

void UGA_ComboAttack::ClearDelegate()
{
	if (ComboAttackNotifyState)
	{
		ComboAttackNotifyState->OnNotifiedBegin.RemoveAll(this);
		ComboAttackNotifyState->OnNotifiedEnd.RemoveAll(this);
	}
	
	if (ComboAttackEndNotify)
	{
		ComboAttackEndNotify->OnNotified.RemoveAll(this);
	}
}

void UGA_ComboAttack::EndAbility(const FGameplayAbilitySpecHandle Handle
															, const FGameplayAbilityActorInfo* ActorInfo
															, const FGameplayAbilityActivationInfo
															ActivationInfo, bool bReplicateEndAbility
															, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility
										, bWasCancelled);
}