﻿#pragma once

#include "CoreMinimal.h"
#include "ProjectNL/GAS/Ability/Utility/BaseInputTriggerAbility.h"
#include "ComboAttack.generated.h"

class UPlayMontageWithEvent;
class UComboAttackNotifyState;
enum class EHandEquipStatus: uint8;

UCLASS()
class PROJECTNL_API UComboAttack : public UBaseInputTriggerAbility
{
	GENERATED_BODY()
public:
	UComboAttack(const FObjectInitializer& ObjectInitializer);

protected:
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

private:
	uint8 ComboIndex = 0;
	uint8 MaxCombo = 0;

	UPROPERTY(EditDefaultsOnly ,meta = (AllowPrivateAccess = true))
	uint8 ComboClearCooldown = 3;

	FTimerHandle ComboClearTimerHandle;

	UFUNCTION()
	void HandleComboNotifyStart(const EHandEquipStatus AttackHand);

	UFUNCTION()
	void HandleComboNotifyEnd(const EHandEquipStatus AttackHand);

	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	void ClearDelegate();

	TObjectPtr<UPlayMontageWithEvent> Task;
	
	TObjectPtr<UComboAttackNotifyState> ComboAttackNotifyState;
};