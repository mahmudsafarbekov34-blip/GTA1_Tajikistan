using UnrealBuildTool;
using System.Collections.Generic;

public class GTA1EditorTarget : TargetRules
{
    public GTA1EditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        ExtraModuleNames.Add("GTA1");
    }
}
