// Copyright Epic Games, Inc. All Rights Reserved.
using UnrealBuildTool;

public class WaveSurvivalEditorTarget : TargetRules
{
	public WaveSurvivalEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("WaveSurvival");
	}
}
