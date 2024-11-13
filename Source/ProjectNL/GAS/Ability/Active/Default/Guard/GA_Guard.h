#pragma once

#include "CoreMinimal.h"
#include "ProjectNL/GAS/Ability/Utility/BaseInputTriggerAbility.h"
#include "GA_Guard.generated.h"

class UAT_PutDamageWithEvent;
class UPlayMontageWithEvent;

UCLASS()
class PROJECTNL_API UGA_Guard : public UBaseInputTriggerAbility
{
	GENERATED_BODY()
	
public:
	UGA_Guard(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

private:
	TObjectPtr<UAT_PutDamageWithEvent> DamageWithEventTask;
	TObjectPtr<UPlayMontageWithEvent> AnimationTask;

	UFUNCTION()
	void GuardDamage(float Damage);

	UFUNCTION()
	void EndBlock(FGameplayTag EventTag, FGameplayEventData EventData);
};
