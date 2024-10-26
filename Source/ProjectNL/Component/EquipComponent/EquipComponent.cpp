#include "EquipComponent.h"

#include "ProjectNL/Helper/StateHelper.h"
#include "ProjectNL/Weapon/BaseWeapon.h"

UEquipComponent::UEquipComponent()
{
	// 플레이어 에게는 테스트 코드가 될 것 (실제로 Class를 주입해서 하는 것이 아닌 인벤토리를 통하기 때문)
	// 다만 플레이어가 아닌 다른 엔티티 기준으로는 테스트 코드가 아닌 컴파일 과정에서 바로 Weapon 객체를 주입하게 됨
	MainWeapon = MainWeaponClass.GetDefaultObject();
	SubWeapon = SubWeaponClass.GetDefaultObject();
	// 혹시 몰라 초기값 할당
	PlayerCombatWeaponState = EPlayerCombatWeaponState::None;
}


// Called when the game starts
void UEquipComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UEquipComponent::UpdateEquipWeaponAnimationData()
{
	SetPlayerCombatWeaponState(FStateHelper::GetCharacterWeaponState(MainWeapon, SubWeapon));
}
