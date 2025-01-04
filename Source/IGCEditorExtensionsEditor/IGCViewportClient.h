
#pragma once

#include "CoreMinimal.h"
#include "EditorViewportClient.h"

class FAdvancedPreviewScene;
class SIGCViewport;
class UIGC;

class FIGCViewportClient : public FEditorViewportClient, public TSharedFromThis<FIGCViewportClient>
{
public:
	
	FIGCViewportClient(FAdvancedPreviewScene& InPreviewScene, const TWeakPtr<SIGCViewport>& InParentViewport);
	~FIGCViewportClient();
	
	virtual void Tick(float DeltaSeconds) override;
	virtual void Draw(const FSceneView* View, FPrimitiveDrawInterface* PDI) override;


private:
	FAdvancedPreviewScene* PreviewScenePtr;
	TWeakPtr<SIGCViewport> ParentViewport;
};