// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Shooter : ModuleRules
{
	public Shooter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(["Shooter"]);

		PublicDependencyModuleNames.AddRange([
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Slate", "SlateCore",
			"OnlineSubsystem",
			
			"Paper2D",
		]);
	}
}
