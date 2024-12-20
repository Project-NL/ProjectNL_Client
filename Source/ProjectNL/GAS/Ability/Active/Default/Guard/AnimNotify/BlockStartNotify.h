#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "BlockStartNotify.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBlockNotifiedSignature);

UCLASS()
class PROJECTNL_API UBlockStartNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	FOnBlockNotifiedSignature OnNotified;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
