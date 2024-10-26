#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectNL/Helper/EnumHelper.h"
#include "ProjectNL/Helper/UtilHelper.h"
#include "BaseWeapon.generated.h"

enum class EUWeaponType : uint8;
enum class EUEquippedHandType : uint8;
enum class EWeaponAttachPosition : uint8;

UCLASS()
class PROJECTNL_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ABaseWeapon();

	void EquipCharacterWeapon(ACharacter* Character, const bool IsMain);
	void UnEquipCharacterWeapon(const bool IsMain);
	void UnEquipCharacterWeapon(ACharacter* Character, const bool IsMain);
	void SwapTwoHandWeapon();

	GETTER(EUEquippedHandType, EquippedHandType)
	GETTER(EWeaponAttachPosition, AttachPosition)
	GETTER(EUWeaponType, WeaponType)
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Assets
	, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* WeaponSkeleton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Assets
		, meta = (AllowPrivateAccess = "true"))
	EUEquippedHandType EquippedHandType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Assets
		, meta = (AllowPrivateAccess = "true"))
	EUWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Assets
		, meta = (AllowPrivateAccess = "true"))
	EWeaponAttachPosition AttachPosition = EWeaponAttachPosition::Back;
};
