#include "GA_Guard.h"

#include "ProjectNL/Helper/GameplayTagHelper.h"
#include "ProjectNL/Helper/StateHelper.h"

UGA_Guard::UGA_Guard(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	
}

void UGA_Guard::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	GetAbilitySystemComponentFromActorInfo()->SetLooseGameplayTagCount(NlGameplayTags::Status_Guard, 1);
}

void UGA_Guard::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	GetAbilitySystemComponentFromActorInfo()->SetLooseGameplayTagCount(NlGameplayTags::Status_Guard, 0);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

