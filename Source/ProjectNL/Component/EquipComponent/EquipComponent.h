#pragma once

#include "CoreMinimal.h"
#include "CombatAnimationData.h"
#include "Components/ActorComponent.h"
#include "ProjectNL/Helper/UtilHelper.h"
#include "EquipComponent.generated.h"

class ABaseWeapon;
class UGameplayEffect;
enum class EPlayerCombatWeaponState : uint8;

// TODO: 해당 컴포넌트의 이름에 대한 고민이 더 필요하다.
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTNL_API UEquipComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEquipComponent();

	void UpdateEquipWeaponAnimationData();

	GETTER(bool, IsFirstEquipWeapon)
	
	// ABP에서 주로 사용함.
	GETTER(TArray<TObjectPtr<UAnimMontage>>, ComboAttackAnim)
	GETTER(TObjectPtr<UAnimMontage>, HeavyAttackAnim);
	GETTER(TObjectPtr<UAnimMontage>, JumpAttackAnim);
	
	GETTER(TObjectPtr<UAnimMontage>, EquipAnim)
	GETTER(TObjectPtr<UAnimMontage>, UnEquipAnim)
	GETTER(TObjectPtr<UAnimMontage>, BlockAnim)
	GETTER(FAnimationByRotation, EvadeAnim)
	GETTER(FAnimationByRotation, StepAnim)

	GETTER(TObjectPtr<ABaseWeapon>, MainWeapon)
	GETTER(TObjectPtr<ABaseWeapon>, SubWeapon)

	
	GETTER(TSubclassOf<UGameplayEffect>, AttackDamageEffect)

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
	
	UPROPERTY(Category="Property|Weapon", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	FDataTableRowHandle CombatAnimByDirectionData;

	// 애니메이션 관련 정보
	UPROPERTY()
	TObjectPtr<UAnimMontage> EquipAnim;

	UPROPERTY()
	TObjectPtr<UAnimMontage> UnEquipAnim;
	
	UPROPERTY()
	TObjectPtr<UAnimMontage> BlockAnim;

	UPROPERTY()
	TArray<TObjectPtr<UAnimMontage>> ComboAttackAnim;
	//강공격
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> HeavyAttackAnim;
	//점프공격
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> JumpAttackAnim;
	
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<UGameplayEffect> AttackDamageEffect;
	
	FAnimationByRotation EvadeAnim;
	
	FAnimationByRotation StepAnim;
};
