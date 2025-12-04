using System.IO;
using UnrealBuildTool;

public class StateMachineExDebugging : ModuleRules
{
	public StateMachineExDebugging(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] 
		{
			Path.Combine(ModuleDirectory, "Public"),
		});
		
		PrivateIncludePaths.AddRange(new string[] 
		{
			Path.Combine(ModuleDirectory, "Private"),
		});

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"StateMachineEx",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"GameplayDebugger",
		});
	}
}