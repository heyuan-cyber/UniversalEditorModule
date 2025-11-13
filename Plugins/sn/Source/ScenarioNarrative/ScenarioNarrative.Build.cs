// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ScenarioNarrative : ModuleRules
{
    public ScenarioNarrative(ReadOnlyTargetRules Target) : base(Target)
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
                "Niagara",
                "Json",
                "JsonUtilities",
                "UMG"

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
                "DialogNarrative"
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
