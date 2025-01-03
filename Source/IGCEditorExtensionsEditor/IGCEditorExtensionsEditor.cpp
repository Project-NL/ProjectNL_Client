// 프로젝트 설정 페이지에서 저작권 정보를 작성하세요.

#include "IGCEditorExtensionsEditor.h"
#include "IGCExtensionCommands.h"
#include "IGCExtensionStyle.h"

#include "Framework/MultiBox/MultiBoxExtender.h"  // 메뉴 확장에 필요한 클래스
#include "Framework/MultiBox/MultiBoxBuilder.h"  // 메뉴 빌더에 필요한 클래스
#include "LevelEditor.h"  // 레벨 에디터에 관련된 클래스

// 모듈을 구현하기 위해 필요한 매크로
IMPLEMENT_MODULE(FIGCEditorExtensionsEditor, IGCEditorExtensionsEditor);

#define LOCTEXT_NAMESPACE "IGCMenu"  // 로컬 텍스트 네임스페이스 정의 (UI 텍스트가 여러 언어로 변환될 수 있도록)

void FIGCEditorExtensionsEditor::StartupModule()
{
    // 스타일 초기화
    FIGCExtensionStyle::Initialize();
    FIGCExtensionStyle::ReloadTextures();

    // 커스텀 명령어 등록
    FIGCExtensionCommands::Register();
    
    // 커맨드 리스트 생성 (UI에서 액션을 수행할 수 있도록 맵핑)
    IGCCommandList = MakeShareable(new FUICommandList());
    IGCCommandList->MapAction(FIGCExtensionCommands::Get().Command1, FExecuteAction::CreateStatic(&FIGCExtensionActions::Action1), FCanExecuteAction());
    IGCCommandList->MapAction(FIGCExtensionCommands::Get().Command2, FExecuteAction::CreateStatic(&FIGCExtensionActions::Action2), FCanExecuteAction());
    IGCCommandList->MapAction(FIGCExtensionCommands::Get().Command3, FExecuteAction::CreateStatic(&FIGCExtensionActions::Action3), FCanExecuteAction());
    IGCCommandList->MapAction(FIGCExtensionCommands::Get().Command4, FExecuteAction::CreateStatic(&FIGCExtensionActions::Action3), FCanExecuteAction());

    // 메뉴 생성 함수 정의
    struct IGCMenu
    {
        static void CreateIGCMenu(FMenuBuilder& MenuBuilder)
        {
            // 첫 번째 섹션 (명령어 1, 2 추가)
            MenuBuilder.BeginSection("IGCSection1", LOCTEXT("IGCMenu", "IGC Menu Section1"));
            MenuBuilder.AddMenuEntry(FIGCExtensionCommands::Get().Command1);
            MenuBuilder.AddMenuEntry(FIGCExtensionCommands::Get().Command2);
            MenuBuilder.EndSection();

            // 두 번째 섹션 (서브메뉴 추가)
            MenuBuilder.BeginSection("IGCSection2", LOCTEXT("IGCMenu", "IGC Menu Section2"));

            // 서브 메뉴 생성
            struct IGCSubMenu
            {
                static void CreateIGCSubMenu(FMenuBuilder& SubMenuBuilder)
                {
                    SubMenuBuilder.AddMenuEntry(FIGCExtensionCommands::Get().Command3);
                    SubMenuBuilder.AddMenuEntry(FIGCExtensionCommands::Get().Command4);
                }
            };

            // 서브 메뉴 추가
            MenuBuilder.AddSubMenu(
                LOCTEXT("IGCMenu", "IGC SubMenu"),
                LOCTEXT("IGCMenu", "IGC SubMenu Tooltip"),
                FNewMenuDelegate::CreateStatic(&IGCSubMenu::CreateIGCSubMenu), false, FSlateIcon());
            MenuBuilder.EndSection();
        }
    };

    // 레벨 에디터 모듈 가져오기
    FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

    // 메뉴 확장 추가
    TSharedPtr<FExtender> IGCMenuExtender = MakeShareable(new FExtender());
    IGCMenuExtender->AddMenuExtension("WindowLayout" /*'WindowLayout' 위치에 추가*/, EExtensionHook::After, IGCCommandList,
        FMenuExtensionDelegate::CreateStatic(&IGCMenu::CreateIGCMenu));

    // 레벨 에디터 메뉴에 확장 적용
    LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(IGCMenuExtender);

    // 툴바 생성 함수 정의
    struct IGCToolbar
    {
        static void CreateIGCToolbar(FToolBarBuilder& ToolbarBuilder)
        {
            // 툴바 섹션 시작
            ToolbarBuilder.BeginSection("IGCToolbar");
            {
                // 툴바 버튼 추가 (각 명령어에 해당하는 버튼)
                ToolbarBuilder.AddToolBarButton(FIGCExtensionCommands::Get().Command1, 
                    NAME_None, TAttribute<FText>(), TAttribute<FText>(), FSlateIcon(FIGCExtensionStyle::GetStyleSetName(), "IGCToolbarIcon.Command1"), NAME_None);
                ToolbarBuilder.AddToolBarButton(FIGCExtensionCommands::Get().Command2,
                    NAME_None, TAttribute<FText>(), TAttribute<FText>(), FSlateIcon(FIGCExtensionStyle::GetStyleSetName(), "IGCToolbarIcon.Command2"), NAME_None);
                ToolbarBuilder.AddToolBarButton(FIGCExtensionCommands::Get().Command3,
                    NAME_None, TAttribute<FText>(), TAttribute<FText>(), FSlateIcon(FIGCExtensionStyle::GetStyleSetName(), "IGCToolbarIcon.Command3"), NAME_None);
                ToolbarBuilder.AddToolBarButton(FIGCExtensionCommands::Get().Command4,
                    NAME_None, TAttribute<FText>(), TAttribute<FText>(), FSlateIcon(FIGCExtensionStyle::GetStyleSetName(), "IGCToolbarIcon.Command4"), NAME_None);
            }
            // 툴바 섹션 끝
            ToolbarBuilder.EndSection();
        }
    };

    // 툴바 확장 추가
    TSharedPtr<FExtender> IGCToolbarExtender = MakeShareable(new FExtender());
    IGCToolbarExtender->AddToolBarExtension("Settings" /*툴바의 Settings 뒤에 추가*/, EExtensionHook::After, IGCCommandList,
        FToolBarExtensionDelegate::CreateStatic(&IGCToolbar::CreateIGCToolbar));

    // 레벨 에디터 툴바에 확장 적용
    LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(IGCToolbarExtender);
}

void FIGCEditorExtensionsEditor::ShutdownModule()
{
    // 명령어 등록 해제
    FIGCExtensionCommands::Unregister();

    // 스타일 종료
    FIGCExtensionStyle::Shutdown();
}

#undef LOCTEXT_NAMESPACE  // 로컬 텍스트 네임스페이스 종료
