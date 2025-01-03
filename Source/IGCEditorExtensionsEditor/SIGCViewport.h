
#pragma once

#include "EngineMinimal.h"
#include "SlateFwd.h"
#include "UObject/GCObject.h"
#include "SEditorViewport.h"
#include "SCommonEditorViewportToolbarBase.h"

class FIGCEditor;
class UIGC;

class SIGCViewport : public SEditorViewport, public FGCObject
{
public:
	SLATE_BEGIN_ARGS(SIGCViewport) {}//
		SLATE_ARGUMENT(TWeakPtr<FIGCEditor>, ParentIGCEditor)
		SLATE_ARGUMENT(UIGC*, ObjectToEdit)
	SLATE_END_ARGS()////이러한 규칙으로 선언에 특수한 매크로를 추가하면 슬레이트 클래스의 인자 값이 추가됩니다. 아래는 위의 위젯을 생성하면서 슬레이트 인자 값을 추가하는 코드입니다.
	//->뷰포트 ,에디터 , ui 감싸는 형식으로 되어 있음 
	void Construct(const FArguments& InArgs);
	SIGCViewport();
	~SIGCViewport();

	
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	TSharedRef<class FAdvancedPreviewScene> GetPreviewScene();

	virtual FString GetReferencerName()const override;
protected:
	virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;

private:

	TWeakPtr<FIGCEditor> IGCEditorPtr;//툴바 /메뉴 등의 사용자 인터페이스 
	TSharedPtr<class FAdvancedPreviewScene> PreviewScene; //독립된 월드 공간의 랜더링을 실행 
	TSharedPtr<class FIGCViewportClient> IGCViewportClient;//각종 랜더링 설정과 뷰포드 관련 데이터를 관리 
	UIGC* IGCObject;//iGC object


	TSharedPtr<SVerticalBox> OverlayTextVerticalBox;


	class UStaticMeshComponent* PreviewMeshComponent;//화면에 보이는 매쉬 컴포넌트 
};