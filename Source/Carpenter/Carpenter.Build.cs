// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Carpenter : ModuleRules
{
	public Carpenter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "DeveloperSettings" });
	}
}
