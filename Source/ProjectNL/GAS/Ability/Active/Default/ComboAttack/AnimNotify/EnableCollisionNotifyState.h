#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ProjectNL/Weapon/BaseWeapon.h"
#include "EnableCollisionNotifyState.generated.h"

class ABaseWeapon;
class UCharacterAnimInstance;
class UGameplayEffect;

UCLASS()
class PROJECTNL_API UEnableCollisionNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category="Options", meta=(AllowPrivateAccess = true))
	uint8 IsHitStop:1 = false;
	
	void StartTraceTriangle(AActor* Owner);
	
	void PerformTriangleTrace(AActor* Owner, FVector Point1, FVector Point2, FVector Point3, TArray<FHitResult>& OutHitResults);

	void MakeTriangleTrace(AActor* Owner, ABaseWeapon* Weapon);

	void ReactToHitActor(AActor* Owner, ABaseWeapon* Weapon, TArray<FHitResult>& HitResults);
};
