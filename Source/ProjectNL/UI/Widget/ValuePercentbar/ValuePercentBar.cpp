#include "ValuePercentBar.h"
#include "Components/ProgressBar.h"

void UValuePercentBar::InitializePercent(const float CurrentValue, const float MaxValue)
{
	PercentNum.Key = CurrentValue;
	PercentNum.Value = MaxValue;

	const float Percent = PercentNum.Key / PercentNum.Value;
	ViewPercentBar->SetPercent(Percent);
	DelayViewPercentBar->SetPercent(Percent);
}

void UValuePercentBar::SetCurrentValue(const float NewValue)
{
	// 새로 들어가는 값이 기존의 값보다 작은 경우 점차 줄어드는 애니메이션 수행
	if (NewValue < PercentNum.Key)
	{
		const float PrevCurrentValue = PercentNum.Key;
		PercentNum.Key = NewValue;
		const float NewPercent = PercentNum.Key / PercentNum.Value;
		ViewPercentBar->SetPercent(NewPercent);
		DecreaseDelayPercentBar(PrevCurrentValue, NewPercent, true);
	}
	else
	{
		// 아니라면 새로운 값 적용 및 Percent 전부 즉시 적용
		PercentNum.Key = NewValue;
		const float NewPercent = PercentNum.Key / PercentNum.Value;
	
		ViewPercentBar->SetPercent(NewPercent);
		DelayViewPercentBar->SetPercent(NewPercent);
	}
}

void UValuePercentBar::SetMaxValue(const float NewValue)
{
	// 아니라면 새로운 값 적용 및 Percent 전부 즉시 적용
	PercentNum.Value = NewValue;
	const float NewPercent = PercentNum.Key / PercentNum.Value;
	
	ViewPercentBar->SetPercent(NewPercent);
	DelayViewPercentBar->SetPercent(NewPercent);
}

void UValuePercentBar::DecreaseDelayPercentBar(const float CurrentPercent, const float FinalPercent, const bool IsStart)
{
	if (CurrentPercent - 0.1 > FinalPercent)
	{
		GetWorld()->GetTimerManager().SetTimer(SetDelayViewPercentTimerHandle, FTimerDelegate::CreateLambda([&]
		{
			DecreaseDelayPercentBar(CurrentPercent - 0.1, FinalPercent, false);
		}), IsStart ? DelayedTime : PercentDecreaseTime, false);
	} else
	{
		GetWorld()->GetTimerManager().ClearTimer(SetDelayViewPercentTimerHandle);
	}
}