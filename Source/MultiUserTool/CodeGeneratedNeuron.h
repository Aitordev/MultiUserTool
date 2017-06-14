// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CustomMeshComponent.h"
#include "CodeGeneratedNeuron.generated.h"

/*
UCLASS()
class MULTIUSERTOOL_API ACodeGeneratedNeuron : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACodeGeneratedNeuron();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
*/
class LocalRef;

USTRUCT(BlueprintType)
struct FGeneratedMeshTriangle
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, Category = Triangle)
		FVector Vertex0;

	UPROPERTY(EditAnywhere, Category = Triangle)
		FVector Vertex1;

	UPROPERTY(EditAnywhere, Category = Triangle)
		FVector Vertex2;
};



class Dendrita {
public:
	TArray<FVector> points;//ESQUELETO
	TArray<FVector> T;
	TArray<FVector> B;
	TArray<FVector> N;
	TArray<FCustomMeshTriangle> triangles;//TRIANGULOS PARA 
	FVector U;
	float L;
	float Ri, Rf;
	FVector Xo, Io, Jo, Ko;
	int nivel;
	int LOD;
	int n, m;//TOPOLOGIA CILINDRICA


	class ACodeGeneratedNeuron *Neurona;
	Dendrita *parent, *childs, *next;

	Dendrita(FVector Xi, FVector Ui, int nivel);
	void Generar(FVector Xi, FVector Ui, TArray<FCustomMeshTriangle> &triangles, int nivel);
	void Update(void);
	



};

class NeuronVertex {
public:
	FVector		Pos;
	FVector		Normal;
	FVector2D	Tcoord;
};

UCLASS()
class MULTIUSERTOOL_API ACodeGeneratedNeuron : public AActor
{
		GENERATED_BODY()
public:
	//TArray<NeuronVertex> Vertices;
	//TArray<NeuronEdge> Edges;
	TArray<FCustomMeshTriangle> triangles;
	float R;
	float Rmax;
	FVector Centro;
	int NumVertex;
	int NumTriangles;
	// Sets default values for this actor's properties
	ACodeGeneratedNeuron();
	void Sample(const TArray<FVector>& points, TArray<FCustomMeshTriangle>& triangles, int segments);
	void GenerarNeurona(void);
	void GenerarSoma(TArray<FCustomMeshTriangle> &triangles);
	void ACodeGeneratedNeuron::GenerarSomaNew();
	void Subdividir(TArray<FCustomMeshTriangle> &input_triangles, TArray<FCustomMeshTriangle> &out_triangles);

	//void GenerarDendrita(void);
	void GenerarDendrita(FVector Xi, FVector U, TArray<FCustomMeshTriangle> &triangles, int nivel);
	void ACodeGeneratedNeuron::GenerarDendrita(LocalRef Refi, TArray<FCustomMeshTriangle> &triangles, int nivel);
	void GenerarDendrita2(FVector Xi, FVector U, float L, int nivel);
	void GenerarTriangulos(int m, FVector *S, FVector *Sa);
	void GenerarSeccion(FVector *S, float r, int m, FVector O, FVector B, FVector N);
	int  GenerarRama(FVector Xi, FRotator Ai, float L, float Ri);
	void AjustarSoma(LocalRef *Refs, int n);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	class UMyCustomMeshComponent *Mesh;
	class UMaterial *TheMaterial;
};

