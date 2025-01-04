// IGCEditor.cpp

#include "IGCEditor.h"
#include "IGCSkillData.h"
#include "SlateOptMacros.h"
#include "PropertyEditorModule.h"
#include "AdvancedPreviewSceneModule.h"

#include "Editor.h"
#include "SIGCViewport.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBorder.h"
#include "Modules/ModuleManager.h"

#include "SSkillTimelineWidget.h" // 우리가 만들 타임라인 위젯 헤더

#define LOCTEXT_NAMESPACE "FIGCEditor"

const FName FIGCEditor::IGCEditorAppIdentifier(TEXT("IGCEditorApp"));
const FName FIGCEditor::ViewportTabId(TEXT("IGCEditor_Viewport"));
const FName FIGCEditor::DetailTabId(TEXT("IGCEditor_Detail"));
const FName FIGCEditor::TimelineTabId(TEXT("IGCEditor_Timeline"));
const FName FIGCEditor::PreviewSceneSettingsTabId(TEXT("IGCEditor_PreviewSceneSettingsTabId"));
void FIGCEditor::InitIGCEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UIGCSkillData* InIGC)
{
    // 편집할 IGC(SkillData) 설정
    SkillDataObject = InIGC;
    SkillDataObject->SetFlags(RF_Transactional); // Undo/Redo 지원

    // 디테일 뷰 생성
    FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    {
        FDetailsViewArgs DetailsViewArgs(/*bUpdateFromSelection=*/false, /*bLockable=*/false, /*bAllowSearch=*/true, 
                                         FDetailsViewArgs::ObjectsUseNameArea, /*bHideSelectionTip=*/false);
        DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
    }

    // 디테일 뷰에 UObject를 연결
    if(DetailsView.IsValid())
    {
        DetailsView->SetObject(SkillDataObject);
    }

    // 뷰포트 위젯 (간단히 SBorder로 대체; 실제로는 SEditorViewport 등을 써야 함)
    ViewportWidget = SNew(SIGCViewport).SkillData(SkillDataObject);
   // .ParentIGCEditor(SharedThis(this))
  

    // 프리뷰 씬 세팅 위젯.
    FAdvancedPreviewSceneModule& AdvancedPreviewSceneModule = FModuleManager::LoadModuleChecked<FAdvancedPreviewSceneModule>("AdvancedPreviewScene");
    AdvancedPreviewSettingsWidget = AdvancedPreviewSceneModule.CreateAdvancedPreviewSceneSettingsWidget(ViewportWidget->GetPreviewScene());
    // 타임라인 위젯 생성
    TimelineWidget = SNew(SSkillTimelineWidget)
        .EditedSkillData(SkillDataObject);

    // 기본 레이아웃 생성
    const TSharedRef<FTabManager::FLayout> EditorDefaultLayout =
        FTabManager::NewLayout("IGCEditor_Layout_v1")
        ->AddArea
        (
            FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
            ->Split
            (
                // Toolbar
                FTabManager::NewStack()
                ->SetSizeCoefficient(0.1f)
                ->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
            )
            ->Split
            (
                FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)
                ->Split
                (
                    // 뷰포트
                    FTabManager::NewStack()
                    ->SetSizeCoefficient(0.7f)
                    ->AddTab(ViewportTabId, ETabState::OpenedTab)
                )
                ->Split
                (
                    // 우측
                    FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)
                    ->Split
                    (
                        // 디테일
                        FTabManager::NewStack()
                        ->SetSizeCoefficient(0.3f)
                        ->AddTab(DetailTabId, ETabState::OpenedTab)
                    )
                    ->Split
                    (
                        // 타임라인
                        FTabManager::NewStack()
                        ->AddTab(TimelineTabId, ETabState::OpenedTab)
                    )
                )
            )
        );

    // FAssetEditorToolkit 초기화
    const bool bCreateDefaultStandaloneMenu = true;
    const bool bCreateDefaultToolbar = true;

    FAssetEditorToolkit::InitAssetEditor(
        Mode,
        InitToolkitHost,
        IGCEditorAppIdentifier,
        EditorDefaultLayout,
        bCreateDefaultStandaloneMenu,
        bCreateDefaultToolbar,
        InIGC
    );
}

FIGCEditor::~FIGCEditor()
{
    // 스마트 포인터 해제
    DetailsView.Reset();
    ViewportWidget.Reset();
    TimelineWidget.Reset();
    AdvancedPreviewSettingsWidget.Reset();
}

// 탭 스포너 등록
void FIGCEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& tabManager)
{
    FAssetEditorToolkit::RegisterTabSpawners(tabManager);

    tabManager->RegisterTabSpawner(ViewportTabId, FOnSpawnTab::CreateSP(this, &FIGCEditor::SpawnTab_Viewport))
        .SetDisplayName(LOCTEXT("ViewportTab", "Viewport"))
        .SetGroup(WorkspaceMenuCategory.ToSharedRef());

    tabManager->RegisterTabSpawner(DetailTabId, FOnSpawnTab::CreateSP(this, &FIGCEditor::SpawnTab_Detail))
        .SetDisplayName(LOCTEXT("DetailTab", "Details"))
        .SetGroup(WorkspaceMenuCategory.ToSharedRef());

    tabManager->RegisterTabSpawner(TimelineTabId, FOnSpawnTab::CreateSP(this, &FIGCEditor::SpawnTab_Timeline))
        .SetDisplayName(LOCTEXT("TimelineTab", "Timeline"))
        .SetGroup(WorkspaceMenuCategory.ToSharedRef());

    tabManager->RegisterTabSpawner(PreviewSceneSettingsTabId, FOnSpawnTab::CreateSP(this, &FIGCEditor::SpawnTab_PreviewSceneSettings))
      .SetDisplayName(LOCTEXT("PreviewSceneSettingsTab", "PreviewSceneSettings"))
      .SetGroup(WorkspaceMenuCategory.ToSharedRef());
}

void FIGCEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& tabManager)
{
    FAssetEditorToolkit::UnregisterTabSpawners(tabManager);

    tabManager->UnregisterTabSpawner(ViewportTabId);
    tabManager->UnregisterTabSpawner(DetailTabId);
    tabManager->UnregisterTabSpawner(TimelineTabId);
    tabManager->UnregisterTabSpawner(PreviewSceneSettingsTabId);
}

// 각 탭 생성 함수
TSharedRef<SDockTab> FIGCEditor::SpawnTab_Viewport(const FSpawnTabArgs& Args)
{
    check(Args.GetTabId() == ViewportTabId);

    return SNew(SDockTab)
    [
        ViewportWidget.ToSharedRef()
    ];
}

TSharedRef<SDockTab> FIGCEditor::SpawnTab_Detail(const FSpawnTabArgs& Args)
{
    check(Args.GetTabId() == DetailTabId);

    return SNew(SDockTab)
    [
        DetailsView.ToSharedRef()
    ];
}

TSharedRef<SDockTab> FIGCEditor::SpawnTab_Timeline(const FSpawnTabArgs& Args)
{
    check(Args.GetTabId() == TimelineTabId);

    return SNew(SDockTab)
    [
        TimelineWidget.ToSharedRef()
    ];
}

TSharedRef<SDockTab> FIGCEditor::SpawnTab_PreviewSceneSettings(const FSpawnTabArgs& Args)
{
    check(Args.GetTabId() == PreviewSceneSettingsTabId);
    return SNew(SDockTab)
        [
            AdvancedPreviewSettingsWidget.ToSharedRef()
        ];
}

FName FIGCEditor::GetToolkitFName() const
{
    return FName("IGCEditor");
}

FText FIGCEditor::GetBaseToolkitName() const
{
    return LOCTEXT("IGCEditorAppLabel", "IGC Editor");
}

FString FIGCEditor::GetWorldCentricTabPrefix() const
{
    return LOCTEXT("IGCEditorWorldCentricTabPrefix", "IGC ").ToString();
}

FLinearColor FIGCEditor::GetWorldCentricTabColorScale() const
{
    return FLinearColor(0.0f, 0.0f, 0.2f, 0.5f);
}

#undef LOCTEXT_NAMESPACE
