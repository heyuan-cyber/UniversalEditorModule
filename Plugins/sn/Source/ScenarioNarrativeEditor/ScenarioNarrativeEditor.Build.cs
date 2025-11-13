// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ScenarioNarrativeEditor : ModuleRules
{
    public ScenarioNarrativeEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange
        (
            new string[]
            {
                // ... add public include paths required here ...
            }
        );

        PrivateIncludePaths.AddRange
        (
            new string[]
            {
                // ... add other private include paths required here ...
            }
        );

        PublicDependencyModuleNames.AddRange
        (
            new[]
            {
                "Core",
                "ScenarioNarrative",
                "CombatAnim",
                "Niagara",
                "Json",
                "UMG"
                // ... add other public dependencies that you statically link with here ...
            }
        );

        PrivateDependencyModuleNames.AddRange
        (
            new[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "UnrealEd",
                "Json",
                "JsonUtilities",
                "SequencerWidgets",
                "PropertyEditor",
                "ContentBrowser",
                "ToolWidgets",
                "AdvancedPreviewScene",
                "CurveEditor",
                "ToolMenus",
                "LevelEditor",
                "ApplicationCore",
                "AssetTools",
                "KismetWidgets",
                "EditorWidgets",
                "AnimGraph",
                "InputCore",
                "Blutility",
                "EditorSubsystem",
                "AssetRegistry",
                "EditorStyle",
                "Projects",
                "EditorFramework",
                "AssetRegistry",
                "ClassViewer",
                "DialogNarrative",
                "CommonUI",
                "SceneOutliner",
                "ComponentVisualizers",
                "RenderCore",
                "TypedElementFramework",
                "TypedElementRuntime",
                "UnrealEd",
                // ... add private dependencies that you statically link with here ...
            }
        );

        DynamicallyLoadedModuleNames.AddRange
        (
            new string[]
            {
                // ... add any modules that your module loads dynamically here ...
            }
        );
    }
}
