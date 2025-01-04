
#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "Toolkits/IToolkitHost.h"

class UIGCSkillData;
class SSkillTimelineWidget;

class FIGCEditor : public FAssetEditorToolkit
{
public:
	static const FName IGCEditorAppIdentifier;
	static const FName ViewportTabId;
	static const FName DetailTabId;
	static const FName TimelineTabId; // 새로 추가할 탭
	static const FName PreviewSceneSettingsTabId;

public:
	void InitIGCEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UIGCSkillData* InIGC);
	virtual ~FIGCEditor();

	// FAssetEditorToolkit 인터페이스 구현
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

protected:
	// 탭 등록 / 언등록
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;

private:
	// 탭을 생성하는 함수들
	TSharedRef<SDockTab> SpawnTab_Viewport(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_Detail(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_Timeline(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_PreviewSceneSettings(const FSpawnTabArgs& Args);

private:
	// 실제 편집할 UObject
	UIGCSkillData* SkillDataObject;
	
	
	// 디테일 뷰
	TSharedPtr<class IDetailsView> DetailsView;

	// 뷰포트 (프리뷰) 위젯
	TSharedPtr<class SIGCViewport> ViewportWidget;

	// 타임라인 위젯
	//TSharedPtr<SSkillTimelineWidget> TimelineWidget;

	// 타임라인 위젯
	TSharedPtr<SSkillTimelineWidget> TimelineWidget;
	
	// 프리뷰 씬 세팅 위젯.
	TSharedPtr<SWidget> AdvancedPreviewSettingsWidget;
	
	/** (예시) 재생 시간 - Timeline과 Viewport에서 공유할 수 있음 */
	float PlaybackTime = 0.0f;

	/** (예시) 실행 중인지 (에디터 상에서 Play 버튼 눌렀다고 가정) */
	bool bIsPlaying = false;
};

