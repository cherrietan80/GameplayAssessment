// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GameplayAssessment : ModuleRules
{
	public GameplayAssessment(ReadOnlyTargetRules Target) : base(Target)
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
			"Slate",
            "GameplayAbilities",
            "GameplayTags",
            "GameplayTasks"
        });

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"GameplayAssessment",
			"GameplayAssessment/Variant_Platforming",
			"GameplayAssessment/Variant_Platforming/Animation",
			"GameplayAssessment/Variant_Combat",
			"GameplayAssessment/Variant_Combat/AI",
			"GameplayAssessment/Variant_Combat/Animation",
			"GameplayAssessment/Variant_Combat/Gameplay",
			"GameplayAssessment/Variant_Combat/Interfaces",
			"GameplayAssessment/Variant_Combat/UI",
			"GameplayAssessment/Variant_SideScrolling",
			"GameplayAssessment/Variant_SideScrolling/AI",
			"GameplayAssessment/Variant_SideScrolling/Gameplay",
			"GameplayAssessment/Variant_SideScrolling/Interfaces",
			"GameplayAssessment/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
