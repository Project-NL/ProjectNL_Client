#include "ComboAttackEndNotify.h"

#include "ProjectNL/Character/BaseCharacter.h"
#include "ProjectNL/Component/EquipComponent/EquipComponent.h"
#include "ProjectNL/Helper/StateHelper.h"

void UComboAttackEndNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	check(MeshComp->GetOwner())
	if (ABaseCharacter* Owner = Cast<ABaseCharacter>(MeshComp->GetOwner()))
	{
		Owner->GetEquipComponent()->MoveNextComboCount();
		UAbilitySystemComponent* ASC = Owner->GetAbilitySystemComponent();
		check(ASC);
        
		FStateHelper::ChangePlayerState(
			Owner->GetAbilitySystemComponent(),NlGameplayTags::State_Attack,
		NlGameplayTags::State_Idle, true);
        
		if (ASC->HasMatchingGameplayTag(NlGameplayTags::State_Attack_Combo))
		{
			FStateHelper::ChangePlayerState(ASC, NlGameplayTags::State_Attack_Combo,
				NlGameplayTags::State_Idle, true);
		} else if (ASC->HasMatchingGameplayTag(NlGameplayTags::State_Attack_Heavy))
		{
			FStateHelper::ChangePlayerState(ASC, NlGameplayTags::State_Attack_Heavy,
				NlGameplayTags::State_Idle, true);
		} else if (ASC->HasMatchingGameplayTag(NlGameplayTags::State_Attack_Jump))
		{
			FStateHelper::ChangePlayerState(ASC, NlGameplayTags::State_Attack_Jump,
				NlGameplayTags::State_Idle, true);
		}
        
		UE_LOG(LogTemp, Log, TEXT("Notify End"));    
	}
	
	Super::Notify(MeshComp, Animation, EventReference);
}
