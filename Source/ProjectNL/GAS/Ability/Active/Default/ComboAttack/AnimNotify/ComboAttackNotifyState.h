#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ComboAttackNotifyState.generated.h"

enum class EHandEquipStatus: uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnComboAttackNotifiedBeginSignature, EHandEquipStatus, AttackHand);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnComboAttackNotifiedSignature, EHandEquipStatus, AttackHand);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnComboAttackNotifiedEndSignature, EHandEquipStatus, AttackHand);

UCLASS()
class PROJECTNL_API UComboAttackNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	FOnComboAttackNotifiedSignature OnNotifiedTick;
	FOnComboAttackNotifiedEndSignature OnNotifiedEnd;
	FOnComboAttackNotifiedBeginSignature OnNotifiedBegin;

	UPROPERTY(EditAnywhere)
	EHandEquipStatus AttackHand;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp
													, UAnimSequenceBase* Animation, float FrameDeltaTime
													, const FAnimNotifyEventReference&
													EventReference) override
	{
		OnNotifiedTick.Broadcast(AttackHand);
		Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	}

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp
												, UAnimSequenceBase* Animation
												, const FAnimNotifyEventReference&
												EventReference) override
	{
		OnNotifiedEnd.Broadcast(AttackHand);
		Super::NotifyEnd(MeshComp, Animation, EventReference);
	}

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp
													, UAnimSequenceBase* Animation, float TotalDuration
													, const FAnimNotifyEventReference&
													EventReference) override
	{
		OnNotifiedBegin.Broadcast(AttackHand);
		Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	}
};