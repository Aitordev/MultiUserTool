// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PaperSpriteActor.h"
#include "MyPaperSpriteActor.generated.h"

/**
 * 
 */
UCLASS()
class MULTIUSERTOOL_API AMyPaperSpriteActor : public APaperSpriteActor
{
	GENERATED_BODY()
public:
	AMyPaperSpriteActor();

	virtual void BeginPlay() override;
	//Para el material
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UPaperSpriteComponent* spriteMesh;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
	//void Enhance(bool enhance);
};
