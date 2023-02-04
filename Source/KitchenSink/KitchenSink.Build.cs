// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class KitchenSink : ModuleRules
{
	public KitchenSink(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });

		PrivateDependencyModuleNames.AddRange(new string[] { "QofL", "AIModule" });

		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PrivatePCHHeaderFile = "prj_precompiled.h";
		MinFilesUsingPrecompiledHeaderOverride = 1;
		bUseUnity = false;
	}
}
