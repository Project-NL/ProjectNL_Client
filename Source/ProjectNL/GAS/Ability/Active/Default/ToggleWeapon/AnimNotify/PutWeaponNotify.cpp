#include "PutWeaponNotify.h"
#include "ProjectNL/Character/BaseCharacter.h"
#include "ProjectNL/Component/EquipComponent/EquipComponent.h"
#include "ProjectNL/Weapon/BaseWeapon.h"

void UPutWeaponNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(
		MeshComp->GetOwner()))
	{
		if (ABaseWeapon* MainWeapon = OwnerCharacter->GetEquipComponent()->GetMainWeapon())
		{
			MainWeapon->UnEquipCharacterWeapon(OwnerCharacter, true);
		}
		if (ABaseWeapon* SubWeapon = OwnerCharacter->GetEquipComponent()->GetSubWeapon())
		{
			SubWeapon->UnEquipCharacterWeapon(OwnerCharacter, false);
		}
	}
	
	Super::Notify(MeshComp, Animation, EventReference);
}
