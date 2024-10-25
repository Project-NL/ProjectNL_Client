#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ValuePercentBar.generated.h"

class UProgressBar;

UCLASS()
class PROJECTNL_API UValuePercentBar : public UUserWidget
{
	GENERATED_BODY()
public:
	void InitializePercent(const float CurrentValue, const float MaxValue);

	void SetCurrentValue(const float NewValue);

	void SetMaxValue(const float NewValue);
protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> ViewPercentBar;
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> DelayViewPercentBar;

private:
	// N초 뒤 실행할 변수 값
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	float DelayedTime = 2;

	// 줄어드는 시간 단위
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	float PercentDecreaseTime = 0.02;
	
	FTimerHandle SetDelayViewPercentTimerHandle;
	
	void DecreaseDelayPercentBar(const float CurrentPercent, const float FinalPercent, const bool IsStart);
	
	// 0번째 현재 값, 1번째 최대 값
	TPair<float, float> PercentNum;
};
