#include "ComboAttackEndNotify.h"

#include "ProjectNL/Character/BaseCharacter.h"
#include "ProjectNL/Component/EquipComponent/EquipComponent.h"

void UComboAttackEndNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	check(MeshComp->GetOwner())
	if (ABaseCharacter* CurrentCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner()))
	{
		CurrentCharacter->GetEquipComponent()->MoveNextComboCount();
	}
	
	Super::Notify(MeshComp, Animation, EventReference);
}
