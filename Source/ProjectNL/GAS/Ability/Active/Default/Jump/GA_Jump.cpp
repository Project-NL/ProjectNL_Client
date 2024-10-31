#include "GA_Jump.h"

#include "GameFramework/PawnMovementComponent.h"
#include "ProjectNL/Character/BaseCharacter.h"

UGA_Jump::UGA_Jump(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool UGA_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle
																		, const FGameplayAbilityActorInfo* ActorInfo
																		, const FGameplayTagContainer* SourceTags
																		, const FGameplayTagContainer* TargetTags
																		, FGameplayTagContainer*
																		OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags
																, OptionalRelevantTags))
	{
		return false;
	}

	return true;
}


void UGA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle
																	, const FGameplayAbilityActorInfo* ActorInfo
																	, const FGameplayAbilityActivationInfo
																	ActivationInfo
																	, const FGameplayEventData* TriggerEventData)
{
	if (ABaseCharacter* OwnerCharacter = CastChecked<ABaseCharacter>(
		ActorInfo->AvatarActor.Get()))
	{
		if (!OwnerCharacter->GetMovementComponent()->IsFalling())
		{
			OwnerCharacter->Jump();
		}
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}
