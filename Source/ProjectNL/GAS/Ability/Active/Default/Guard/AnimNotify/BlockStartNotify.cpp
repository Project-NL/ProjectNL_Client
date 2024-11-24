#include "BlockStartNotify.h"

#include "ProjectNL/Character/BaseCharacter.h"
#include "ProjectNL/Helper/StateHelper.h"

void UBlockStartNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner()))
	{
		UNLAbilitySystemComponent* ASC = Cast<UNLAbilitySystemComponent>(Character->GetAbilitySystemComponent());
		check(ASC)
		FStateHelper::ChangePlayerState(ASC, NlGameplayTags::Status_Block, NlGameplayTags::Status_Guard);
	}
	Super::Notify(MeshComp, Animation, EventReference);
}
