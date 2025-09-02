// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PTP : ModuleRules
{
	public PTP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"PTP",
			"PTP/Variant_Platforming",
			"PTP/Variant_Platforming/Animation",
			"PTP/Variant_Combat",
			"PTP/Variant_Combat/AI",
			"PTP/Variant_Combat/Animation",
			"PTP/Variant_Combat/Gameplay",
			"PTP/Variant_Combat/Interfaces",
			"PTP/Variant_Combat/UI",
			"PTP/Variant_SideScrolling",
			"PTP/Variant_SideScrolling/AI",
			"PTP/Variant_SideScrolling/Gameplay",
			"PTP/Variant_SideScrolling/Interfaces",
			"PTP/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
