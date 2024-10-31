#include "GA_Sprint.h"

#include "AbilitySystemComponent.h"

UGA_Sprint::UGA_Sprint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bActivateAbilityInputTrigger = true;
	bIsActivated = false;
}

bool UGA_Sprint::CanActivateAbility(const FGameplayAbilitySpecHandle Handle
																, const FGameplayAbilityActorInfo* ActorInfo
																, const FGameplayTagContainer* SourceTags
																, const FGameplayTagContainer* TargetTags
																, FGameplayTagContainer* OptionalRelevantTags)
const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags
																, OptionalRelevantTags))
	{
		return false;
	}

	return true;
}
void UGA_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (bIsActivated)
	{
		return;
	}
	bIsActivated = true;
	if (BuffEffect)
	{
		EffectContext = GetAbilitySystemComponentFromActorInfo()->
			MakeEffectContext();
		EffectContext.AddSourceObject(GetAvatarActorFromActorInfo());

		SpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
			BuffEffect, 1.0f, EffectContext);
		
		if (SpecHandle.IsValid())
		{
			GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToSelf(
				*SpecHandle.Data.Get());
		}
	}
}

void UGA_Sprint::CancelAbility(const FGameplayAbilitySpecHandle Handle
														, const FGameplayAbilityActorInfo* ActorInfo
														, const FGameplayAbilityActivationInfo
														ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	bIsActivated = false;
	GetAbilitySystemComponentFromActorInfo()->
		RemoveActiveGameplayEffectBySourceEffect(BuffEffect, GetAbilitySystemComponentFromActorInfo());
}

void UGA_Sprint::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	bIsActivated = false;
}
