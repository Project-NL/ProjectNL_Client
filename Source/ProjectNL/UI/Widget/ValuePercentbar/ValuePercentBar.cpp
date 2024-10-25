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
		
		const float PrevPercent = PrevCurrentValue / PercentNum.Value;
		const float NewPercent = PercentNum.Key / PercentNum.Value;
		
		ViewPercentBar->SetPercent(NewPercent);
		DecreaseDelayPercentBar(PrevPercent, NewPercent, true);
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
	DelayViewPercentBar->SetPercent(CurrentPercent);
	if (CurrentPercent - 0.01 > FinalPercent)
	{
		const float NewPercent = CurrentPercent - 0.01;
		// TODO: SetTimer를 사용할 때 기존의 Parameter 정보가 유실되는 이슈 확인
		// 정확한 원인이 뭔지 파악할 필요가 있음
		GetWorld()->GetTimerManager().SetTimer(SetDelayViewPercentTimerHandle, FTimerDelegate::CreateLambda([&]
		{
			DecreaseDelayPercentBar(NewPercent, FinalPercent, false);
		}), IsStart ? DelayedTime : PercentDecreaseTime, false);
	} else
	{
		GetWorld()->GetTimerManager().ClearTimer(SetDelayViewPercentTimerHandle);
	}
}