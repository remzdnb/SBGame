using UnrealBuildTool;
using System.Collections.Generic;

public class SBGameServerTarget : TargetRules
{
    public SBGameServerTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Server;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.Add("SBGame");
    }
}