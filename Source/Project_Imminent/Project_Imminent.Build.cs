// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Project_Imminent : ModuleRules
{
	public Project_Imminent(TargetInfo Target)
	{
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "AIModule" });
    }
}
