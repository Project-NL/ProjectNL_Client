#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AnimNotifyManager.generated.h"

UCLASS()
class PROJECTNL_API UAnimNotifyManager : public UObject
{
	GENERATED_BODY()
public:
	// TODO: FindNotifyByClass, FindNotifyStateByClass 둘을 같이 써야하거나, 하나의 애니메이션에서
	// 연속으로 사용해야한다면 불필요한 순회가 발생할 수 있다. 그렇기 때문에 한번만 가져오는 경우에는 사용하면 좋지만,
	// 그것이 아니라면 굳이 이걸 사용하기보다 Animation->Notifies를 가지고 활용하는 것이 더 효율적이다.
	// 추가로 해당 매니저의 메소드들을 하나의 메소드에서 사용하는 경우 사용하는 횟수만큼 NotifyState가 중복 등록되는 이슈를
	// 확인한 상황. 말이 안되는 버그지만 혹시 모르니 다시 확인할 필요는 있음
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