// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiUserTool.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "MyPaperSpriteActor.h"


AMyPaperSpriteActor::AMyPaperSpriteActor()
{
	PrimaryActorTick.bCanEverTick = true;
	struct FConstructorStatics {
		ConstructorHelpers::FObjectFinderOptional<UPaperSprite> spriteMesh;
		FConstructorStatics()
			: spriteMesh(TEXT("/Game/Sprites/Texturas/sprite_3.sprite_3"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;
	// Create static mesh component
	spriteMesh = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteMesh0"));
	spriteMesh->SetSprite(ConstructorStatics.spriteMesh.Get());

	RootComponent = spriteMesh;
	SetReplicates(true);
}


// Called when the game starts or when spawned
void AMyPaperSpriteActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMyPaperSpriteActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}




