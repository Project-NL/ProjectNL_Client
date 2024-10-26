#include "EquipComponent.h"

#include "GameFramework/Character.h"
#include "ProjectNL/Helper/StateHelper.h"
#include "ProjectNL/Weapon/BaseWeapon.h"

UEquipComponent::UEquipComponent()
{
	// 혹시 몰라 초기값 할당
	PlayerCombatWeaponState = EPlayerCombatWeaponState::None;
}

void UEquipComponent::BeginPlay()
{
	Super::BeginPlay();
	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		MainWeapon = GetWorld()->SpawnActor<ABaseWeapon>(MainWeaponClass);
		SubWeapon = GetWorld()->SpawnActor<ABaseWeapon>(SubWeaponClass);
		if (IsFirstEquipWeapon)
		{
			if (MainWeapon)
            {
             	MainWeapon->EquipCharacterWeapon(Character, true);
            }
            if (SubWeapon)
            {
             	SubWeapon->EquipCharacterWeapon(Character, false);
            }
		} else
		{
			if (MainWeapon)
            {
            	MainWeapon->UnEquipCharacterWeapon(Character, true);
            }
            if (SubWeapon)
            {
            	SubWeapon->UnEquipCharacterWeapon(Character, false);
            }	
		}
	}
	
	UpdateEquipWeaponAnimationData();
}

void UEquipComponent::UpdateEquipWeaponAnimationData()
{
	PlayerCombatWeaponState = FStateHelper::GetCharacterWeaponState(MainWeapon, SubWeapon);
}
