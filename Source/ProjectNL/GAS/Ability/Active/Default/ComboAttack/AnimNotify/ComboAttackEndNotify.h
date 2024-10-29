#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ComboAttackEndNotify.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnComboAttackEndNotifiedSignature);

UCLASS()
class PROJECTNL_API UComboAttackEndNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	FOnComboAttackEndNotifiedSignature OnNotified;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override
	{
		OnNotified.Broadcast();
		Super::Notify(MeshComp, Animation, EventReference);
	}
};
