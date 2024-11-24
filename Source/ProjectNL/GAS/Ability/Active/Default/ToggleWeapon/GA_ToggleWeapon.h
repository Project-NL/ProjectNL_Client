#pragma once

#include "CoreMinimal.h"
#include "ProjectNL/GAS/Ability/Utility/BaseInputTriggerAbility.h"
#include "GA_ToggleWeapon.generated.h"

class UPlayMontageWithEvent;
class UGrabWeaponNotify;

UCLASS()

class PROJECTNL_API UGA_ToggleWeapon : public UBaseInputTriggerAbility
{
	GENERATED_BODY()
	
public:
	UGA_ToggleWeapon(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle
															, const FGameplayAbilityActorInfo* ActorInfo
															, const FGameplayAbilityActivationInfo
															ActivationInfo
															, const FGameplayEventData*
															TriggerEventData) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle
																	, const FGameplayAbilityActorInfo* ActorInfo
																	, const FGameplayTagContainer* SourceTags
																	, const FGameplayTagContainer* TargetTags
																	, FGameplayTagContainer* OptionalRelevantTags)
	const override;

private:
	// TODO: 이름에 대해 다시 생각해볼 필요는 있음.
	bool ToggleCombatState();
	
	UFUNCTION()
	void EndToggleWeapon(FGameplayTag EventTag, FGameplayEventData EventData);
	
	TObjectPtr<UPlayMontageWithEvent> AnimationTask;

	UPROPERTY()
	TObjectPtr<UAnimNotify> ToggleWeaponNotify;

};
