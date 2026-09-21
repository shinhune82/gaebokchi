using UnrealBuildTool;
using System.Collections.Generic;

public class GaebokchiTarget : TargetRules
{
	public GaebokchiTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("Gaebokchi");
	}
}
