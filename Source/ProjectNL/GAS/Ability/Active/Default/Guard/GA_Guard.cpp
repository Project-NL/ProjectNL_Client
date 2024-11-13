#include "GA_Guard.h"

#include "ProjectNL/Character/Player/PlayerCharacter.h"
#include "ProjectNL/GAS/Ability/Utility/AT_PutDamageWithEvent.h"
#include "ProjectNL/GAS/Attribute/PlayerAttributeSet.h"
#include "ProjectNL/Helper/GameplayTagHelper.h"
#include "ProjectNL/Helper/StateHelper.h"

UGA_Guard::UGA_Guard(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	
}

void UGA_Guard::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	GetAbilitySystemComponentFromActorInfo()->SetLooseGameplayTagCount(NlGameplayTags::Status_Guard, 1);

	DamageWithEventTask = UAT_PutDamageWithEvent::InitialEvent(this);
	DamageWithEventTask->OnDamage.AddDynamic(this, &ThisClass::GuardDamage);
	DamageWithEventTask->ReadyForActivation();
}

void UGA_Guard::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	GetAbilitySystemComponentFromActorInfo()->SetLooseGameplayTagCount(NlGameplayTags::Status_Guard, 0);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGA_Guard::GuardDamage(float Damage)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (UPlayerAttributeSet* PAS = Player->PlayerAttributeSet.Get())
		{
			// TODO: 추후 / 10 대신 방어용 로직 넣을 예정
			PAS->SetHealth(PAS->GetHealth() - Damage / 10);
		}
	}
}


