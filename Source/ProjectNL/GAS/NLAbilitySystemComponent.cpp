#include "NLAbilitySystemComponent.h"

#include "NLAbilitySystemInitializationData.h"
#include "Ability/Utility/BaseInputTriggerAbility.h"


UNLAbilitySystemComponent::UNLAbilitySystemComponent()
{
	ReplicationMode = EGameplayEffectReplicationMode::Mixed;
	SetIsReplicatedByDefault(true);
}

void UNLAbilitySystemComponent::InitializeAbilitySystem(
	const FNLAbilitySystemInitializationData& InitData)
{
	if (GetIsInitialized())
	{
		return;
	}

	if (GetOwnerRole() == ROLE_Authority)
	{
		if (!InitData.GameplayAbilities.IsEmpty())
		{
			for (TSubclassOf<UBaseInputTriggerAbility> Ability : InitData.
					GameplayAbilities)
			{
				UBaseInputTriggerAbility* InputAbility = Ability->GetDefaultObject<
					UBaseInputTriggerAbility>();

				GiveAbility(FGameplayAbilitySpec(
					Ability, InputAbility->GetAbilityLevel()
					, static_cast<uint32>(InputAbility->GetInputID()), this));
			}
		}
		
		if (IsValid(InitData.FirstNameAbility))
		{
			UGameplayAbility* Ability = InitData.FirstNameAbility->GetDefaultObject<
				UGameplayAbility>();
			GiveAbility(FGameplayAbilitySpec(
				Ability, Ability->GetAbilityLevel(), -1, this));
		}

		if (IsValid(InitData.SecondNameAbility))
		{
			UGameplayAbility* Ability = InitData.SecondNameAbility->GetDefaultObject<
				UGameplayAbility>();
			GiveAbility(FGameplayAbilitySpec(
				Ability, Ability->GetAbilityLevel(), -1, this));
		}

		if (IsValid(InitData.ThirdNameAbility))
		{
			UGameplayAbility* Ability = InitData.ThirdNameAbility->GetDefaultObject<
				UGameplayAbility>();
			GiveAbility(FGameplayAbilitySpec(
				Ability, Ability->GetAbilityLevel(), -1, this));
		}

		if (!InitData.GameplayEffects.IsEmpty())
		{
			for (TSubclassOf<UGameplayEffect> Effect : InitData.GameplayEffects)
			{
				ApplyGameplayEffectToSelf(Effect.GetDefaultObject(), 1
																	, MakeEffectContext());
			}
		}
	}

	if (!InitData.GameplayTags.IsEmpty())
	{
		AddLooseGameplayTags(InitData.GameplayTags);
	}
	
	SetIsInitialized(true);
}

void UNLAbilitySystemComponent::ReceiveDamage(const float Damage) const
{
	OnDamage.Broadcast(Damage);
}
