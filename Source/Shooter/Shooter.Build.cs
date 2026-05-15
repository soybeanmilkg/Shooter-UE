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
			"OnlineSubsystem",
			"Paper2D",
			"ModularGameplayActors",
			"CommonGame", "CommonUser", "CommonLoadingScreen",
			"GameplayMessageRuntime", "GameSettings", "GameExperience",
		]);
	}
}
