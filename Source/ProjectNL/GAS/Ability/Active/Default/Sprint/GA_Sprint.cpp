#include "GA_Sprint.h"

#include "AbilitySystemComponent.h"

UGA_Sprint::UGA_Sprint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bActivateAbilityInputTrigger = true;
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

// TODO: Release될때만 실행되는 이슈 수정
void UGA_Sprint::OnTriggeredInputAction(const FInputActionValue& Value)
{
	Super::OnTriggeredInputAction(Value);

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
	GetAbilitySystemComponentFromActorInfo()->
		RemoveActiveGameplayEffectBySourceEffect(BuffEffect
																						, GetAbilitySystemComponentFromActorInfo());
	EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility, false);
}