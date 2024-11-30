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
	TObjectPtr<UPlayMontageWithEvent> AttackAnimTask;
	
	UPROPERTY(EditDefaultsOnly ,meta = (AllowPrivateAccess = true))
	float ComboClearCooldown;
	
	UPROPERTY(EditDefaultsOnly ,meta = (AllowPrivateAccess = true))
	float HoldDurationCooldown;

	FTimerHandle ComboClearTimerHandle;
	
	FDateTime InputPressedTime;

	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);
	
	UFUNCTION()
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	bool CanAttack() const;
};
