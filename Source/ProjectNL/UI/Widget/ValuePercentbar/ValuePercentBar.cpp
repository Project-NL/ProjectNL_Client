#include "ValuePercentBar.h"
#include "Components/ProgressBar.h"

void UValuePercentBar::OnWidgetRebuilt()
{
	ViewPercentBar->SetFillColorAndOpacity(MainColor);
	DelayViewPercentBar->SetFillColorAndOpacity(SubColor);
}

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
		DelayPercentInfo = {PrevPercent, NewPercent};
		DecreaseDelayPercentBar();
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
	
	DelayPercentInfo.FinalPercent = NewPercent;
	ViewPercentBar->SetPercent(NewPercent);
	DelayViewPercentBar->SetPercent(NewPercent);
}

void UValuePercentBar::DecreaseDelayPercentBar()
{

	// 타이머를 등록해 DelayViewPercentBar 정보가 계속 줄어들게 설정
	GetWorld()->GetTimerManager().SetTimer(SetDelayViewPercentTimerHandle, FTimerDelegate::CreateLambda([&]
	{
		// TODO: 추후 CurveTable을 이용해 수학 로직을 넣어 커스텀하게 조정하기 
		DelayPercentInfo.CurrentPercent -= 0.01;
		
		DelayViewPercentBar->SetPercent(DelayPercentInfo.CurrentPercent);
		
		// 만약 현재 Percent값과 최종적으로 반영되야할 값이랑 같거나 작게되는 경우 sync를 맞추고
		// timer를 clear 시키는 작업을 수행한다.
		if (DelayPercentInfo.CurrentPercent <= DelayPercentInfo.FinalPercent)
		{
			DelayPercentInfo.CurrentPercent = DelayPercentInfo.FinalPercent;
			DelayViewPercentBar->SetPercent(DelayPercentInfo.CurrentPercent);
			GetWorld()->GetTimerManager().ClearTimer(SetDelayViewPercentTimerHandle);
		}
	}), PercentDecreaseTime, true, DelayedTime);
}