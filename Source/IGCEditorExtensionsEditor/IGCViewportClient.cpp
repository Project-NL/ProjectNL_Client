
#include "IGCViewportClient.h"
#include "SIGCViewport.h"
#include "IGCEditor.h"
#include "AdvancedPreviewScene.h"

//#include "EditorStyleSet.h"
#include "Editor/EditorPerProjectUserSettings.h"
#include "AssetViewerSettings.h"


// FIGCViewportClient::FIGCViewportClient(TWeakPtr<class FIGCEditor> ParentIGCEditor, const TSharedRef<class FAdvancedPreviewScene>& AdvPreviewScene, const TSharedRef<class SIGCViewport>& IGCViewport, UIGC * ObjectToEdit)
// 	: FEditorViewportClient(nullptr, &AdvPreviewScene.Get(), StaticCastSharedRef<SEditorViewport>(IGCViewport))
// 	, IGCEditorPtr(ParentIGCEditor)
// 	, IGCEditorViewportPtr(IGCViewport)
// 	, IGCObject(ObjectToEdit)
// {
// 	SetViewMode(VMI_Lit);
//
// 	AdvancedPreviewScene = static_cast<FAdvancedPreviewScene*>(PreviewScene);
//
// 	SetViewLocation(FVector(0.0f, 3.0f, 2.0f));
// 	SetViewRotation(FRotator(-45.0f, -90.0f, 0.0f));
// 	SetViewLocationForOrbiting(FVector::ZeroVector, 500.0f);
// }
FIGCViewportClient::FIGCViewportClient(FAdvancedPreviewScene& InPreviewScene, const TWeakPtr<SIGCViewport>& InParentViewport)
		   : FEditorViewportClient(nullptr, &InPreviewScene)
		   , PreviewScenePtr(&InPreviewScene)
		   , ParentViewport(InParentViewport)
{
	SetViewLocation(FVector(100,100,50));
	SetViewRotation(FRotator(0,-90,0));
	// 기타 설정
}

FIGCViewportClient::~FIGCViewportClient()
{
}

void FIGCViewportClient::Tick(float DeltaSeconds)
{
	FEditorViewportClient::Tick(DeltaSeconds);
	PreviewScenePtr->GetWorld()->Tick(LEVELTICK_All, DeltaSeconds);
}

void FIGCViewportClient::Draw(const FSceneView * View, FPrimitiveDrawInterface * PDI)
{
	FEditorViewportClient::Draw(View, PDI);
}
