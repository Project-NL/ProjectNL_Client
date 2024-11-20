#pragma once

#include "CoreMinimal.h"
#include "ProjectNL/GAS/Ability/Utility/BaseInputTriggerAbility.h"
#include "GA_ComboAttack.generated.h"
class ABaseCharacter;
class UAT_HeavyAttack;
class UComboAttackEndNotify;
class UPlayMontageWithEvent;
class UComboAttackNotifyState;
enum class EHandEquipStatus: uint8;

UCLASS()
class PROJECTNL_API UGA_ComboAttack : public UBaseInputTriggerAbility
{
	GENERATED_BODY()
public:
	UGA_ComboAttack(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle
															, const FGameplayAbilityActorInfo* ActorInfo
															, const FGameplayAbilityActivationInfo
															ActivationInfo
															, const FGameplayEventData*
															TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle
													, const FGameplayAbilityActorInfo* ActorInfo
													, const FGameplayAbilityActivationInfo ActivationInfo
													, bool bReplicateEndAbility
													, bool bWasCancelled) override;
	

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	// 헤비 어택 실행 함수
	void ExecuteHeavyAttack();
	// 콤보 어택 실행 함수
	void ExecuteComboAttack();
	// 점프 어택 실행 함수
	void ExecuteJumpAttack();
	
private:
	UPROPERTY()
	TObjectPtr<ABaseCharacter> CurrentCharacter;
	uint8 ComboIndex = 0;
	uint8 MaxCombo = 0;

	UPROPERTY(EditDefaultsOnly ,meta = (AllowPrivateAccess = true))
	uint8 ComboClearCooldown;

	FTimerHandle ComboClearTimerHandle;

	FDateTime InputPressedTime;

	// 헤비 어택을 위한 임계값 시간 (초 단위)
	FDateTime InputReleasedTime;
	
	UFUNCTION()
	void HandleComboNotifyStart(const EHandEquipStatus AttackHand);

	UFUNCTION()
	void HandleComboNotifyEnd(const EHandEquipStatus AttackHand);

	UFUNCTION()
	void HandleComboEndNotify();

	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnCompletedAbility(FGameplayTag EventTag, FGameplayEventData EventData);
	UFUNCTION()
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);
	// UFUNCTION()
	// void OnCompleteCallback();
	void ClearDelegate();
	//콤보 어택
	UPROPERTY()
	TObjectPtr<UPlayMontageWithEvent> ComboAttackTask;
	UPROPERTY()
	TObjectPtr<UComboAttackNotifyState> ComboAttackNotifyState;
	UPROPERTY()
	TObjectPtr<UComboAttackEndNotify> ComboAttackEndNotify;
	bool ComboAttackPerform;
	//강공격
	UPROPERTY()
	TObjectPtr<UPlayMontageWithEvent> HeavyAttackTask;

	FTimerHandle HeavyAttackTimerHandle;

	uint8 HeavyAttackPerform:1;

	//점프 공격
	TObjectPtr<UPlayMontageWithEvent> JumpAttackTask;
	
};
