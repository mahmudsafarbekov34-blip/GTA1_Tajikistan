using UnrealBuildTool;
using System.Collections.Generic;

public class GTA1Target : TargetRules
{
    public GTA1Target(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        ExtraModuleNames.Add("GTA1");
    }
}
