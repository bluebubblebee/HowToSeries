// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HowTo : ModuleRules
{
	public HowTo(ReadOnlyTargetRules Target) : base(Target)
	{
		CppStandard = CppStandardVersion.Cpp20;  // Set to C++20
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore"});

        PrivateDependencyModuleNames.AddRange(new string[] {});

    }
}
