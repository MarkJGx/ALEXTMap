// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class ALEXTMapLibrary : ModuleRules
{
	public ALEXTMapLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
		PublicIncludePaths.Add("$(PluginDir)/Source/ThirdParty/ALEXTMapLibrary/ALEX/src/core/");
	}
}
