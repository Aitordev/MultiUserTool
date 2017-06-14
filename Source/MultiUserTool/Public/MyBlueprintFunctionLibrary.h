// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MULTIUSERTOOL_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/** What is the Player's current musical skill level? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerMusicSkill)
	int32 MusicSkillLevel;
		
};
