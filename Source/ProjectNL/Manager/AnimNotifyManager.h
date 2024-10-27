#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AnimNotifyManager.generated.h"

UCLASS()
class PROJECTNL_API UAnimNotifyManager : public UObject
{
	GENERATED_BODY()
public:
	template<typename AnimNotify>
	UFUNCTION() static TObjectPtr<AnimNotify> FindNotifyByClass(const TObjectPtr<UAnimMontage> Animation)
	{
		if (!Animation)
		{
			return nullptr;
		}

		for (FAnimNotifyEvent NotifyEvent : Animation->Notifies)
		{
			if (const auto AnimationNotify = Cast<AnimNotify>(NotifyEvent.Notify))
			{
				return AnimationNotify;
			}
		}

		return nullptr;
	}

	template<typename AnimNotifyState>
	UFUNCTION() static TObjectPtr<AnimNotifyState> FindNotifyStateByClass(const TObjectPtr<UAnimMontage> Animation)
	{
		if (!Animation)
		{
			return nullptr;
		}

		for (FAnimNotifyEvent NotifyEvent : Animation->Notifies)
		{
			if (const auto AnimationNotify = Cast<AnimNotifyState>(NotifyEvent.NotifyStateClass))
			{
				return AnimationNotify;
			}
		}

		return nullptr;
	}
};