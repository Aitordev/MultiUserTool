// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Neuron.h"
#include "NeuronForest.generated.h"

UCLASS()
class MULTIUSERTOOL_API ANeuronForest : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ANeuron **NEURONAS;
	int NUM_NEURONAS;
	int NumNeuronMeshes;
	// Sets default values for this actor's properties
	ANeuronForest();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	int LoadNeuronsMeshes(char *path);
	void CreateRandomNeuronForest(int n, float d);
	void LoadNeuronForest(char* fich);


	
	
};
