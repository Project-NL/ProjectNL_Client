#include "EquipComponent.h"

#include "CombatAnimationData.h"
#include "GameFramework/Character.h"
#include "ProjectNL/Character/BaseCharacter.h"
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
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner()))
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
	SetAnimationsByWeaponState();
}

// TODO: 추후 별도의 Manager로 옮겨야 할 지 고려할 필요 있음
void UEquipComponent::SetAnimationsByWeaponState()
{
	const FString AttackAnimName = FEnumHelper::GetClassEnumKeyAsString(
		PlayerCombatWeaponState) + "AttackAnim";

	if (const FCombatAnimationData* Animation = CombatAnimData.DataTable->FindRow<
		FCombatAnimationData>(*AttackAnimName, ""))
	{
		ComboAttackAnim = Animation->AnimGroup;
	} else
	{
		ComboAttackAnim = TArray<UAnimMontage*>();
	}

	const FString EquipAnimRowName = FEnumHelper::GetClassEnumKeyAsString(
		PlayerCombatWeaponState) + "EquipAnim";
	if (const FCombatAnimationData* NewEquipAnim = CombatAnimData.DataTable->FindRow<
		FCombatAnimationData>(*EquipAnimRowName, ""))
	{
		EquipAnim = NewEquipAnim->AnimGroup.Top();
	} else
	{
		EquipAnim = nullptr;
	}
	
	const FString UnEquipAnimRowName = FEnumHelper::GetClassEnumKeyAsString(
		PlayerCombatWeaponState) + "UnEquipAnim";
	if (const FCombatAnimationData* NewUnEquipAnim = CombatAnimData.DataTable->FindRow<
		FCombatAnimationData>(*UnEquipAnimRowName, ""))
	{
		UnEquipAnim = NewUnEquipAnim->AnimGroup.Top();
	} else
	{
		UnEquipAnim = nullptr;
	}

	const FString EvadeAnimRowName = FEnumHelper::GetClassEnumKeyAsString(
		PlayerCombatWeaponState) + "EvadeAnim";
	if (const FCombatAnimationByRotationData* NewEvadeAnim = CombatAnimByDirectionData.DataTable->FindRow<
		FCombatAnimationByRotationData>(*EvadeAnimRowName, ""))
	{
		EvadeAnim = NewEvadeAnim->AnimGroup;
	} else
	{
		EvadeAnim = {};
	}
}
