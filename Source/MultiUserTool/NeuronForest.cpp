// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiUserTool.h"
#include "Funciones.h"
#include "LightControls.h"
#include "Local.h"
#include "NeuronForest.h"
#include "Neuron.h"

extern UWorld *GWORLD;// GetWorld();
					  //ANeuron *BOSQUE_NEURONAS[64000];
					  //int NUM_NEURONAS = 0;
//int NNEUR = 0;
/* AHORA EN Funciones.cpp
float frand()
{
	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	return r;
}

FVector RandomVector(float sz)
{
	return FVector(sz*frand(), sz*frand(), sz*frand());
}

FRotator RandomRotator(float sz)
{
	return FRotator(sz*frand(), sz*frand(), sz*frand());
}
*/

//void loadNeuronForest(char* fich);

int ANeuronForest::LoadNeuronsMeshes(char *path)
{
	int n = 0;

	return n;
}

void ANeuronForest::CreateRandomNeuronForest(int n, float d)
{
	/*
	if (GWORLD) {
	LoadNeuronForest("C:/Users/Javier/Desktop/Scenes/resultados.txt");
	LoadNeuronForest("C:/Users/Javier/Desktop/Scenes/LowRes_4.txt");
	}

	return;
	*/


	FVector Pos;
	FRotator Rot;
	float R = d;// 300.0f;
	int nx = n;
	int ny = n;
	int nz = n;
	int max_neuronas = (2 * nx + 1)*(2 * ny + 1)*(2 * nz + 1);
	BOSQUE_NEURONAS = (ANeuron **)malloc(max_neuronas*sizeof(ANeuron *));
	if (GWORLD) {
		for (int k = -nz; k <= nz; k++) {
			for (int i = -nx; i <= nx; i++) {
				for (int j = -ny; j <= ny; j++) {
					Pos = FVector(R * i, R * j, R * k) + RandomVector(0.5f*R);
					Rot = RandomRotator(90.0f);
					BOSQUE_NEURONAS[NUM_NEURONAS++] = GWORLD->SpawnActor<ANeuron>(ANeuron::StaticClass(), Pos, Rot);
				}
			}
		}
	}
}

void ANeuronForest::LoadNeuronForest(char* fich) {
	FVector Pos;
	FRotator Rot;
	float X, Y, Z;
	int RotY;
	int S;
	//	FString TipoNeurona;

	FILE *f = fopen(fich, "r");
	if (f) {
		float version;
		int n;
		char NombreNeurona[32];
		fscanf(f, "%f", &version);
		fscanf(f, "%d", &n);
		BOSQUE_NEURONAS = (ANeuron **)malloc(n*sizeof(ANeuron *));
		while (fscanf(f, "%f %f %f %d %d %s", &X, &Y, &Z, &RotY, &S, NombreNeurona) != EOF) {
			//AÑADIR LA NEURONA
			Pos = FVector(X, Y, Z);
			Rot = FRotator(0.0f, (float)RotY, 0.0f);
			BOSQUE_NEURONAS[NUM_NEURONAS++] = GWORLD->SpawnActor<ANeuron>(ANeuron::StaticClass(), Pos, Rot);
		}
		fclose(f);
	}
}

// Sets default values
ANeuronForest::ANeuronForest()
{

	BOSQUE_NEURONAS = NULL;
	NUM_NEURONAS = 0;
	NumNeuronMeshes = 0;
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//CreateRandomNeuronForest(3, 300.0f);


}

// Called when the game starts or when spawned
void ANeuronForest::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ANeuronForest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*******************************************************************************
USAMOS UNA FUNCION GLOBAL POR AHORA
*******************************************************************************/
//ANeuron **BOSQUE_NEURONAS = NULL;// [64000];
//int NUM_NEURONAS = 0;


void CreateRandomNeuronForest(int n, float d)
{
	extern int NNEUR;
	FVector Pos;
	FRotator Rot;
	float R = d;// 300.0f;
	int nx = n;
	int ny = n;
	int nz = n;
	int max_neuronas = (2 * nx + 1)*(2 * ny + 1)*(2 * nz + 1);
	NNEUR = max_neuronas;
	BOSQUE_NEURONAS = (ANeuron **)malloc(max_neuronas*sizeof(ANeuron *));
	if (GWORLD) {
		for (int k = -nz; k <= nz; k++) {
			for (int i = -nx; i <= nx; i++) {
				for (int j = -ny; j <= ny; j++) {
					Pos = FVector(R * i, R * j, R * k) + RandomVector(0.5f*R);
					Rot = RandomRotator(90.0f);
					BOSQUE_NEURONAS[NUM_NEURONAS++] = GWORLD->SpawnActor<ANeuron>(ANeuron::StaticClass(), Pos, Rot);
				}
			}
		}
	}
}

void LoadNeuronForest(char* fich) {
	FVector Pos;
	FRotator Rot;
	float X, Y, Z;
	int RotY;
	int S;
	extern int NNEUR;

	//FString TipoNeurona;

	FILE *f = fopen(fich, "r");
	if (f) {
		float version;
		int n;
		char NombreNeurona[32];
		fscanf(f, "%f", &version);
		fscanf(f, "%d", &n);
		NNEUR = n;
		BOSQUE_NEURONAS = (ANeuron **)malloc(n*sizeof(ANeuron *));
		while (fscanf(f, "%f %f %f %d %d %s", &X, &Y, &Z, &RotY, &S, NombreNeurona) != EOF) {
			//AÑADIR LA NEURONA
			Pos = FVector(X, Y, Z);
			Rot = FRotator(0.0f, (float)RotY, 0.0f);
			BOSQUE_NEURONAS[NUM_NEURONAS++] = GWORLD->SpawnActor<ANeuron>(ANeuron::StaticClass(), Pos, Rot);
		}
		fclose(f);
	}
}


