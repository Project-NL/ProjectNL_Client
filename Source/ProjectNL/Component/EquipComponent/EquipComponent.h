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
	GETTER(TArray<TObjectPtr<UAnimMontage>>, ComboAttackAnim)
protected:
	virtual void BeginPlay() override;

	void SetAnimationsByWeaponState();

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EPlayerCombatWeaponState PlayerCombatWeaponState;
	
	UPROPERTY(Category="Property", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	bool IsFirstEquipWeapon = false;

	// 무기 관련 정보
	UPROPERTY(Category="Property|Weapon", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<ABaseWeapon> MainWeaponClass;

	UPROPERTY(Category="Property|Weapon", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<ABaseWeapon> SubWeaponClass;

	TObjectPtr<ABaseWeapon> MainWeapon;
	
	TObjectPtr<ABaseWeapon> SubWeapon;

	// 애니메이션 관련 정보 Table
	// 아무 DB나 다 들어가게 되어있지만 실제 사용 DB는 CombatAnimationData.h 기준으로 들어감
	// TODO: 타입 제한이 가능한지 다시 check할 필요가 있음
	UPROPERTY(Category="Property|Weapon", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	FDataTableRowHandle CombatAnimData;

	// 애니메이션 관련 정보
	UPROPERTY(BlueprintReadOnly, Category = Animation
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> EquipAnim;

	UPROPERTY(BlueprintReadOnly, Category = Animation
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> UnEquipAnim;

	UPROPERTY(BlueprintReadOnly, Category = Animation
		, meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UAnimMontage>> ComboAttackAnim;
};
