// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "IGCSkillData.h"

/**
 * 
 */
class SSkillTimelineWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSkillTimelineWidget){}
	SLATE_ARGUMENT(UIGCSkillData*, EditedSkillData)
		// 뷰포트에도 전달할 “바인딩” 형태 시간
	  SLATE_ATTRIBUTE(float, CurrentTime)
SLATE_END_ARGS()

void Construct(const FArguments& InArgs);

	// 여기서 Tick 등 재생 로직을 처리할 수도 있음
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:
	// 편집 중인 SkillData
	UIGCSkillData* SkillData;
    
	// 간단히 Steps를 표시하기 위한 리스트뷰 or Slate
	TSharedPtr<SListView<TSharedPtr<FSkillStepData>>> StepsListView;

	// 외부와 공유할 CurrentTime (바인딩)
	TAttribute<float> CurrentTime;

	// 내부 Slider 값
	float TimeSliderValue = 0.f;

	// StepsListView에 들어갈 데이터(래핑)
	TArray<TSharedPtr<FSkillStepData>> StepItems;

	// Slider Value -> CurrentTime 갱신
	void OnTimeSliderChanged(float NewValue);

	// 리스트뷰 항목 생성 함수
	TSharedRef<ITableRow> OnGenerateRowForStep(TSharedPtr<FSkillStepData> Item, const TSharedRef<STableViewBase>& OwnerTable);

	// Steps 배열 -> StepItems 변환
	void RefreshStepItems();
};
