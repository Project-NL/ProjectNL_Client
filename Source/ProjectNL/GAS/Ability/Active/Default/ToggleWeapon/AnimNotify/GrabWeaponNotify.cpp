#include "GrabWeaponNotify.h"

#include "ProjectNL/Character/BaseCharacter.h"
#include "ProjectNL/Component/EquipComponent/EquipComponent.h"
#include "ProjectNL/Weapon/BaseWeapon.h"


void UGrabWeaponNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(
		MeshComp->GetOwner()))
	{
		if (ABaseWeapon* MainWeapon = OwnerCharacter->GetEquipComponent()->GetMainWeapon())
		{
			MainWeapon->EquipCharacterWeapon(OwnerCharacter, true);
		}
		if (ABaseWeapon* SubWeapon = OwnerCharacter->GetEquipComponent()->GetSubWeapon())
		{
			SubWeapon->EquipCharacterWeapon(OwnerCharacter, false);
		}
	}
	
	Super::Notify(MeshComp, Animation, EventReference);
}
