#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectNL/Helper/UtilHelper.h"
#include "EquipComponent.generated.h"

class ABaseWeapon;
enum class EPlayerCombatWeaponState : uint8;

// TODO: 해당 컴포넌트의 이름에 대한 고민이 더 필요하다.
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTNL_API UEquipComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEquipComponent();

	void UpdateEquipWeaponAnimationData();
	
	// ABP에서 주로 사용함.
	GETTER(EPlayerCombatWeaponState, PlayerCombatWeaponState)
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EPlayerCombatWeaponState PlayerCombatWeaponState;
	
	UPROPERTY(Category="Property", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	bool IsFirstEquipWeapon = false;
	
	UPROPERTY(Category="Property|Weapon", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	FDataTableRowHandle CombatAnimData;
	
	UPROPERTY(Category="Property|Weapon", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<ABaseWeapon> MainWeaponClass;

	UPROPERTY(Category="Property|Weapon", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<ABaseWeapon> SubWeaponClass;

	TObjectPtr<ABaseWeapon> MainWeapon;
	
	TObjectPtr<ABaseWeapon> SubWeapon;
};
