#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PutWeaponNotify.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPutWeaponNotifiedSignature);

UCLASS()
class PROJECTNL_API UPutWeaponNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	FOnPutWeaponNotifiedSignature OnNotified;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override
	{
		OnNotified.Broadcast();
		Super::Notify(MeshComp, Animation, EventReference);
	}
};
