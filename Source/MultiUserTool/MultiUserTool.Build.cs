// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class MultiUserTool : ModuleRules
{
	public MultiUserTool(TargetInfo Target)
	{
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "RHI", "RenderCore", "ShaderCore", "AdvancedSessions", "OnlineSubsystem", "OnlineSubsystemUtils", "Networking", "Sockets", "OnlineSubsystemNull", "ImageWrapper", "Http", "Json" });
        PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "Sockets", "Networking","OnlineSubsystemNull", "CustomMeshComponent", "ProceduralMeshComponent", "Slate", "SlateCore", "Http", "Json"});
        PrivateIncludePathModuleNames.AddRange(new string[] { "CustomMeshComponent" ,"ProceduralMeshComponent", "Paper2D" });
        DynamicallyLoadedModuleNames.AddRange(new string[] { "NetworkReplayStreaming", "NullNetworkReplayStreaming", "HttpNetworkReplayStreaming" });
    }
}
