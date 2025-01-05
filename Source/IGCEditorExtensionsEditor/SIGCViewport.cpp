
#include "SIGCViewport.h"
#include "IGCViewportClient.h"
#include "AdvancedPreviewScene.h"
#include "EditorStyleSet.h"
#include "IGCSkillData.h"
#include "Animation/DebugSkelMeshComponent.h"
#include "Editor/AnimGraph/Public/AnimPreviewInstance.h"
#include "Framework/Commands/Contexts/UIContentContext.h"
#include "Misc/MapErrors.h"

#define LOCTEXT_NAMESPACE "IGCViewport"

void SIGCViewport::Construct(const FArguments& InArgs)
{
    CurrentTime = InArgs._CurrentTime;
    SkillData = InArgs._SkillData;

    // 미리보기 씬 생성
    PreviewScenePtr = MakeShareable(new FAdvancedPreviewScene(FPreviewScene::ConstructionValues()));

    // 에디터 뷰포트 SEditorViewport 초기화
    SEditorViewport::Construct(SEditorViewport::FArguments());

    // SkeletalMeshComponent 생성 & Scene에 등록
    InitPreviewScene();
}

SIGCViewport::~SIGCViewport()
{
    // 뷰포트 종료시 정리
    if(PreviewSkeletalMeshComp)
    {
        PreviewSkeletalMeshComp->DestroyComponent();
        PreviewSkeletalMeshComp = nullptr;
    }
    PreviewScenePtr.Reset();
}
void SIGCViewport::SetPreviewAnimationAsset(UAnimationAsset* AnimAsset, bool bEnablePreview /*= true*/)
{
    if (PreviewSkeletalMeshComp)
    {

        PreviewSkeletalMeshComp->EnablePreview(bEnablePreview, AnimAsset);
    }
    
}

void SIGCViewport::InitPreviewScene()
{
    if(!PreviewScenePtr.IsValid())
    {
        return;
    }

    // 스켈레탈 메시 컴포넌트 생성
    PreviewSkeletalMeshComp = NewObject<UDebugSkelMeshComponent>(GetTransientPackage(), UDebugSkelMeshComponent::StaticClass());
    // 메시 설정 (필요하다면 SkillData에서 SkeletalMesh나 AnimBP를 참조해서 로드)
    // PreviewSkeletalMeshComp->SetSkeletalMesh(SomeSkeletalMesh);
    // 스켈레탈 메시 로드
    USkeletalMesh* SkeletalMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Sword_Animations/Demo/Mannequins/Character/Meshes/SKM_Manny.SKM_Manny"));
    if (SkeletalMesh)
    {
        PreviewSkeletalMeshComp->SetSkeletalMesh(SkeletalMesh);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("스켈레탈 메시를 로드하지 못했습니다."));
        return;
    }
    PreviewSkeletalMeshComp->SetActive(true);
  //  PreviewSkeletalMeshComp->SetAnimationMode(EAnimationMode::AnimationSingleNode);
    UAnimationAsset* AnimationAsset = LoadObject<UAnimSequence>(nullptr, TEXT("/Game/Assets/Character/Animation/001_OnlyOneHandWeapon/Block/AS_OnlyOneHandWeapon_Guard.AS_OnlyOneHandWeapon_Guard"));
    if (AnimationAsset)
    {
        PreviewSkeletalMeshComp->SetAnimationMode(EAnimationMode::AnimationSingleNode);
        SetPreviewAnimationAsset(AnimationAsset,true);
        //PreviewSkeletalMeshComp->PreviewInstance->SetPosition(PlayPosition);
        PreviewSkeletalMeshComp->PreviewInstance->SetPlaying(true);
    }
   
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("애니메이션 애셋을 로드하지 못했습니다."));
        // 애니메이션 로드에 실패해도 계속 진행할 수 있습니다.
    }
    // 애니메이션 단일 재생 모드
  

    UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(NULL, TEXT("/Engine/EngineMeshes/SM_MatPreviewMesh_01.SM_MatPreviewMesh_01"), NULL, LOAD_None, NULL);
    UMaterialInterface* BaseMaterial = LoadObject<UMaterialInterface>(NULL, TEXT("/Engine/EditorMeshes/ColorCalibrator/M_ChromeBall.M_ChromeBall"), NULL, LOAD_None, NULL);

    PreviewMeshComponent = NewObject<UStaticMeshComponent>(GetTransientPackage(), NAME_None, RF_Transient);
    PreviewMeshComponent->SetStaticMesh(StaticMesh);
    PreviewMeshComponent->SetMaterial(0, BaseMaterial);
    // 프리뷰씬에 컴포넌트 등록
    PreviewScenePtr->AddComponent(PreviewSkeletalMeshComp, FTransform::Identity);
}

void SIGCViewport::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
    SEditorViewport::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

    // 현재 타임라인 시간 구하기
    float TimeNow = CurrentTime.Get(0.f);

    // 예: 첫 번째 Step의 AnimSequence를 재생한다거나...
    if(SkillData.IsValid() && PreviewSkeletalMeshComp)
    {
        const UIGCSkillData* Data = SkillData.Get();
        if(Data->Steps.Num() > 0)
        {
            const FSkillStepData& Step0 = Data->Steps[0];
            if(Step0.AnimationAsset.IsValid())
            {
                UAnimSequenceBase* AnimSeq = Cast<UAnimSequenceBase>(Step0.AnimationAsset.LoadSynchronous());
                if(AnimSeq)
                {
                    // 이 애니메이션을 “TimeNow” 위치로
                    PreviewSkeletalMeshComp->SetAnimation(AnimSeq);
                    PreviewSkeletalMeshComp->SetPosition(TimeNow, false); // 루프=false
                }
            }
        }
    }
}

TSharedRef<FEditorViewportClient> SIGCViewport::MakeEditorViewportClient()
{
    ViewportClient = MakeShareable(new FIGCViewportClient( *PreviewScenePtr.Get(), SharedThis(this)));//

    return ViewportClient.ToSharedRef();
    // ViewportClient = MakeShareable(new FIGCViewportClient(*PreviewScenePtr.Get(), SharedThis(this)));
    // return ViewportClient.ToSharedRef();
}

void SIGCViewport::AddReferencedObjects(FReferenceCollector& Collector)
{
    Collector.AddReferencedObject(PreviewMeshComponent);
}



FString SIGCViewport::GetReferencerName() const
{
    return "SIGCViewport";
}

TSharedRef<class FAdvancedPreviewScene> SIGCViewport::GetPreviewScene()
{
    return PreviewScenePtr.ToSharedRef();
}
// TSharedPtr<SWidget> SIGCViewport::MakeViewportToolbar()
// {
//     return SAssignNew(ViewportToolbar, SAnimViewportToolBar, TabBodyPtr.Pin(), SharedThis(this))
//         .Visibility(EVisibility::SelfHitTestInvisible)
//         .Cursor(EMouseCursor::Default)
//         .Extenders(Extenders)
//         .ContextName(FUIContentContext::ContextName)
//         .PreviewProfileController(MakeShared<FPreviewProfileController>())
//         .ShowShowMenu(bShowShowMenu)
//         .ShowLODMenu(bShowLODMenu)
//         .ShowPlaySpeedMenu(bShowPlaySpeedMenu)
//         .ShowFloorOptions(bShowFloorOptions)
//         .ShowTurnTable(bShowTurnTable)
//         .ShowPhysicsMenu(bShowPhysicsMenu);
// }

#undef LOCTEXT_NAMESPACE

