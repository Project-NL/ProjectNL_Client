
#pragma once

#include "CoreMinimal.h"
#include "SEditorViewport.h"
#include "SlateFwd.h"
#include "UObject/GCObject.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Animation/AnimSequenceBase.h"
#include "EditorViewportClient.h"
#include "Framework/Commands/UICommandList.h"
#include "SCommonEditorViewportToolbarBase.h"
// ▼ 가능하다면 Persona Commands, AnimViewportCommands 등을 포함 ▼
// #include "AnimViewportMenuCommands.h"
// #include "AnimViewportShowCommands.h"
// #include "AnimViewportLODCommands.h"
// #include "AnimViewportPlaybackCommands.h"
class FAdvancedPreviewScene;
class FIGCEditor;
class UIGC;

class SIGCViewport : public SEditorViewport, public FGCObject
{
public:
	SLATE_BEGIN_ARGS(SIGCViewport){}
	// 타임라인 시간 등을 받아올 수도 있음
	SLATE_ATTRIBUTE(float, CurrentTime)
	SLATE_ARGUMENT(TWeakObjectPtr<class UIGCSkillData>, SkillData)
SLATE_END_ARGS()

void Construct(const FArguments& InArgs);

	/** 소멸자 */
	virtual ~SIGCViewport();

	// 뷰포트 갱신(에디터 틱마다 호출)
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	// 프리뷰씬 가져오기(툴바용 등)
	virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;
	virtual EVisibility GetTransformToolbarVisibility() const { return EVisibility::Visible; }
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	virtual FString GetReferencerName() const override ;
	TSharedRef<class FAdvancedPreviewScene> GetPreviewScene();
	
	// 뷰포트에서 사용할 PreviewScene
	TSharedPtr<FAdvancedPreviewScene> PreviewScenePtr;

private:
	// 에디터 뷰포트 클라이언트
	TSharedPtr<class FIGCViewportClient> ViewportClient;

	// 우리가 실제 표시할 스켈레탈 메시 컴포넌트
	USkeletalMeshComponent* PreviewSkeletalMeshComp = nullptr;


	class UStaticMeshComponent* PreviewMeshComponent=nullptr;
	// 재생 시간 (Timeline에서 받아옴)
	TAttribute<float> CurrentTime;

	// 스킬데이터 (Steps 등)
	TWeakObjectPtr<class UIGCSkillData> SkillData;

	// 내부에서 사용할 초기화 함수
	void InitPreviewScene();
	
};