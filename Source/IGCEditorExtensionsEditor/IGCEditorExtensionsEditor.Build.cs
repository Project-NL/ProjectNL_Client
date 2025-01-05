// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class IGCEditorExtensionsEditor : ModuleRules
{
	public IGCEditorExtensionsEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicIncludePathModuleNames.AddRange(
			new string[] {
				"Engine",
				"SkeletonEditor",
				"AnimationEditor",
				"AdvancedPreviewScene",
			}
		);
		PublicDependencyModuleNames.AddRange(
			new string[] 
			{ 
				"Core", 
				"CoreUObject",
                "SlateCore",
                "Slate",
                "UnrealEd",
                "LevelEditor",
                "ProjectNL",
                "Engine",
                "AdvancedPreviewScene",
                "EditorStyle",
                "InputCore", 
                "Persona",
                "AnimGraphRuntime" ,
                "AnimationEditMode",
            }
		);

        PrivateIncludePathModuleNames.AddRange(
                new string[] {
                "AssetRegistry",
                "MainFrame",
                "DesktopPlatform",
                "AssetTools",
                "MeshReductionInterface",
                "SequenceRecorder",
                "AnimationBlueprintEditor",
                "EditorInteractiveToolsFramework",
                }
            );

        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "AppFramework",
                "AnimationModifiers",
                "AnimationBlueprintLibrary",
                "AudioEditor",
                "Core",
                "CoreUObject",
                "ApplicationCore",
                "Slate",
                "SlateCore",
                "ContentBrowserData",
                "EditorStyle",
                "Engine",
                "EditorFramework",
                "UnrealEd",
                "GraphEditor",
                "InputCore",
                "Kismet",
                "KismetWidgets",
                "AnimGraph",
                "PropertyEditor",
                "EditorWidgets",
                "DesktopWidgets",
                "BlueprintGraph",
                "RHI",
                "InterchangeEngine",
                "Json",
                "JsonUtilities",
                "ClothingSystemEditorInterface",
                "ClothingSystemRuntimeInterface",
                "ClothingSystemRuntimeCommon",
                "AnimGraphRuntime",
                "CommonMenuExtensions",
                "PinnedCommandList",
                "RenderCore",
                "SkeletalMeshUtilitiesCommon",
                "NaniteUtilities",
                "ToolMenus",
                "CurveEditor",
                "SequencerWidgets",
                "TimeManagement",
                "Sequencer",
                "StatusBar",
                "ToolWidgets",
                "InteractiveToolsFramework",
                "ContentBrowser",
                "SkeletalMeshDescription",
            }
        );

        DynamicallyLoadedModuleNames.AddRange(
            new string[] {
                "Documentation",
                "MainFrame",
                "DesktopPlatform",
                "SkeletonEditor",
                "AssetTools",
                "AnimationEditor",
                "MeshReductionInterface",
                "SequenceRecorder",
            }
        );
    }
}
