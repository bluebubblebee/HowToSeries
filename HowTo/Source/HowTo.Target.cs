// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class HowToTarget : TargetRules
{
	public HowToTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
		bOverrideBuildEnvironment = true;
        CppStandard = CppStandardVersion.Cpp20;
        ExtraModuleNames.Add("HowTo");
	}
}
