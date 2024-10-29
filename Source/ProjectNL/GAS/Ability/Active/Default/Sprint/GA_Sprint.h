#pragma once

#include "CoreMinimal.h"
#include "ProjectNL/GAS/Ability/Utility/BaseInputTriggerAbility.h"
#include "GA_Sprint.generated.h"


UCLASS()
class PROJECTNL_API UGA_Sprint : public UBaseInputTriggerAbility
{
	GENERATED_BODY()

public:
	UGA_Sprint(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle
																	, const FGameplayAbilityActorInfo* ActorInfo
																	, const FGameplayTagContainer* SourceTags
																	, const FGameplayTagContainer* TargetTags
																	, FGameplayTagContainer* OptionalRelevantTags)
	const override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle
														, const FGameplayAbilityActorInfo* ActorInfo
														, const FGameplayAbilityActivationInfo
														ActivationInfo
														, bool bReplicateCancelAbility) override;

	virtual void OnTriggeredInputAction(const FInputActionValue& Value) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="Ability|Effect"
		, meta=(AllowPrivateAccess = true))
	TSubclassOf<UGameplayEffect> BuffEffect;

	FGameplayEffectContextHandle EffectContext;
	FGameplayEffectSpecHandle SpecHandle;
};