// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
//#include "Funciones.h"
#include "CustomMeshComponent.h"
#include "CodeGeneratedNeuron.h"
#include "ImpostorNeuronForest.generated.h"

//VERTICE SOPORTE PARA 
struct ImpostorVertex {
	FVector Position;
	FVector2D Tuv;
};

//BILLBOARDING (QUAD)
struct ImpostorItem {
	FVector Pos;
	float sz;
	FVector a, b, c, d;
	int TextureIndex;
};

//class FCustomMeshTriangle;

UCLASS()
class MULTIUSERTOOL_API AImpostorNeuronForest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AImpostorNeuronForest();
	

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
public:
	UMaterial *ItemMaterial;
	TArray<FCustomMeshTriangle> triangles;
	UCustomMeshComponent* Mesh;
	ImpostorItem *Impostores;
	int max_neuronas;

	void UpdateNeuronForest(FVector CamPos, FVector Cx, FVector Cy, float Fov);
	void CreateRandomNeuronForest(int n, float d);
	void GenerateMesh();

	
	
};
