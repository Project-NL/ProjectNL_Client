#include "NLAbilitySystemComponent.h"

#include "NLAbilitySystemInitializationData.h"
#include "Ability/Utility/BaseInputTriggerAbility.h"
#include "Attribute/PlayerAttributeSet.h"
#include "ProjectNL/Character/Player/PlayerCharacter.h"
#include "ProjectNL/Helper/GameplayTagHelper.h"


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
	// 추가적인 동작이 필요할 때 (ex. 특정 이벤트 실행)
	OnDamage.Broadcast(Damage);
	// TODO: 추후 플레이어 말고도 다른 엔티티나 액터 등에 대한 예외도 처리해야함.
	// 현재는 플레이어만 AttributeSet이 장착되어 있기 때문에 이렇게 처리됨.
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetAvatarActor()))
    {
		UPlayerAttributeSet* PAS = Player->PlayerAttributeSet.Get();
		check(PAS)
		
		if (HasMatchingGameplayTag(NlGameplayTags::Status_Guard))
		{
			// TODO: 추후 / 10 대신 방어용 로직 넣을 예정
			PAS->SetHealth(PAS->GetHealth() - Damage / 10);
			return;
		}

		PAS->SetHealth(PAS->GetHealth() - Damage);
    }
}
