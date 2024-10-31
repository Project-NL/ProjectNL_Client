#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GrabWeaponNotify.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGrabWeaponNotifiedSignature);

UCLASS()
class PROJECTNL_API UGrabWeaponNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	FOnGrabWeaponNotifiedSignature OnNotified;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override
	{
		OnNotified.Broadcast();
		Super::Notify(MeshComp, Animation, EventReference);
	}
};
