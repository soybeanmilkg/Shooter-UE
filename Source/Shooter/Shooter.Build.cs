// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Shooter : ModuleRules
{
	public Shooter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(["Shooter"]);

		PublicDependencyModuleNames.AddRange([
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
			"Slate", "SlateCore", "UMG", "CommonUI",

			"ModularGameplay", "ModularGameplayActors",
			"CommonGame", "CommonUser", "CommonLoadingScreen", "CommonPools",
			"GameplayMessageRuntime", "GameSettings", "GameExperience",

			"GameplayTags",
			"OnlineSubsystem",
			"Paper2D",
			"ControlFlows",
			"PropertyPath",
		]);
	}
}
