// SkillTimelineWidget.cpp

#include "SSkillTimelineWidget.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SSlider.h"

void SSkillTimelineWidget::Construct(const FArguments& InArgs)
{
	SkillData = InArgs._EditedSkillData;
	CurrentTime = InArgs._CurrentTime; // 바인딩

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			// 예: 현재 시간을 표시
			SNew(STextBlock)
			.Text_Lambda([this] {
				return FText::AsNumber(TimeSliderValue);
			})
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			// 슬라이더: 0 ~ 5초 범위 예시
			SNew(SSlider)
			.Value(TimeSliderValue)
			.OnValueChanged(this, &SSkillTimelineWidget::OnTimeSliderChanged)
			.MinValue(0.f)
			.MaxValue(5.f)
		]
		// 여기서 Steps를 리스트로 보여주거나, 커스텀 드로잉할 수도 있음
	];
}

void SSkillTimelineWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	// 여기서 현재 편집 중인 타임(시뮬레이션 시간)을 업데이트하고,
	// 스텝별로 애니메이션/파티클 미리보기 등을 해줄 수 있습니다.
    
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
}

TSharedRef<ITableRow> SSkillTimelineWidget::OnGenerateRowForStep(
	TSharedPtr<FSkillStepData> Item, 
	const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FSkillStepData>>, OwnerTable)
	[
		SNew(STextBlock)
		.Text(FText::FromString(
			FString::Printf(TEXT("Step: Start=%.2f, End=%.2f, Action=%s"), 
				Item->StartTime, Item->EndTime, *Item->ActionType)
		))
	];
}

void SSkillTimelineWidget::RefreshStepItems()
{
	StepItems.Empty();

	if (SkillData)
	{
		// Steps 배열을 TSharedPtr로 변환하여 보관
		for (auto& Step : SkillData->Steps)
		{
			// 임시 복사본을 만드는 게 아닌, 주소를 직접 래핑하는 경우:
			TSharedPtr<FSkillStepData> StepPtr = MakeShareable(new FSkillStepData(Step));
			StepItems.Add(StepPtr);
		}
	}
}
void SSkillTimelineWidget::OnTimeSliderChanged(float NewValue)
{
	TimeSliderValue = NewValue;

	// SlateAttribute<float> 형태의 CurrentTime에 반영
	// 보통은 바인딩을 양방향으로 제어해야 해서, TAttribute가 아닌 TSharedRef<FMyValue> 형태를 쓰기도 함
	// 여기서는 간단히 "메모리 참조"로 갱신한다고 가정
	if (CurrentTime.IsBound())
	{
		// 바운드된 TAttribute<float>를 직접 바꿀 수는 없으니
		// 보통은 외부에서 “Setter”를 주입받아 호출하거나, 
		// 또는 TSharedPtr<float> 등을 공유해서 갱신하는 식으로 구현합니다.
	}
}