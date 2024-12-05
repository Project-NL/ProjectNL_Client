#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	TSet<AActor*>& GetHitActorsReference();
	GETTER(EUEquippedHandType, EquippedHandType)
	GETTER(EWeaponAttachPosition, AttachPosition)
	GETTER(EUWeaponType, WeaponType)
	GETTER(USkeletalMeshComponent*, WeaponSkeleton)

	GETTER_SETTER(FVector, PrevStartLocation)
	GETTER_SETTER(FVector, PrevEndLocation)
	GETTER_SETTER(TSet<AActor*>, HitActors)
	GETTER_SETTER(FVector, LastAttackDirection)

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
	EWeaponAttachPosition AttachPosition;

	UPROPERTY()
	TSet<AActor*> HitActors;

	// 캐릭터별로 관리할 변수 선언
	FVector PrevStartLocation;
	FVector PrevEndLocation;
	FVector LastAttackDirection;
};
