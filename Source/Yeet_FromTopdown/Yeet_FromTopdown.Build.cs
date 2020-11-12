// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Yeet_FromTopdown : ModuleRules
{
	public Yeet_FromTopdown(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        //PublicIncludePaths.AddRange(new string[] { "Engine/Plugins/Runtime/ProceduralMeshComponent/Source/ProceduralMeshComponent/Public", "Engine/Plugins/Runtime/ProceduralMeshComponent/Source/ProceduralMeshComponent/Public/Classes" });
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "ProceduralMeshComponent" });

		//PrivateIncludePaths.AddRange(new string[] { "ProceduralMeshComponent/Private", "ProceduralMeshComponent/Classes" });
		//PrivateDependencyModuleNames.AddRange(new string[] { "ProceduralMeshComponent" });
		
	}
}
