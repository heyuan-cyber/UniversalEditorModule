using UnrealBuildTool;

public class UMEEditor : ModuleRules
{
    public UMEEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", 
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "UMERuntime",
                "UnrealEd",              // GEditor / AssetEditorSubsystem
                "AssetTools",            // IAssetTools, RegisterAdvancedAssetCategory
                "AssetRegistry",         // FAssetCategoryPath
                "EditorSubsystem",       // UAssetEditorSubsystem
                "ToolMenus",             // 菜单系统
                "AssetDefinition",       // UAssetDefinition 基类
                "EngineAssetDefinitions",// UAssetDefinition_Blueprint 
                "GameplayTags", 
            }
        );
    }
}