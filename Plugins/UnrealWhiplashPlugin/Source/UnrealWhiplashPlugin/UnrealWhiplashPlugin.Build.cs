// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnrealWhiplashPlugin : ModuleRules
{
	public UnrealWhiplashPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"DynamicMesh",
				"GeometryCore",
				"GeometryFramework"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"Projects",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

        PublicDelayLoadDLLs.Add("WhipLib-static.dll");

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            RuntimeDependencies.Add(".\\internal\\WhipLib\\bin\\x64\\WhipLib_static.dll");
        }
    }
}
