// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiUserTool.h"
#include "Funciones.h"
#include "Local.h"
#include "ImpostorNeuronForest.h"


// Sets default values
AImpostorNeuronForest::AImpostorNeuronForest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UCustomMeshComponent>(TEXT("CustomMesh"));
	CreateRandomNeuronForest(20, 200.0f);
	GenerateMesh();
	Mesh->SetCustomMeshTriangles(triangles);
	
	if (1) {
		static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/Material/NeuronSpriteMaterial.NeuronSpriteMaterial'"));
		if (Material.Object != NULL)
		{
			ItemMaterial = (UMaterial*)Material.Object;
		}
	}
	
	Mesh->SetMaterial(0,ItemMaterial);
	RootComponent = Mesh;
	SetReplicates(true);

}

// Called when the game starts or when spawned
void AImpostorNeuronForest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AImpostorNeuronForest::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}




void AImpostorNeuronForest::CreateRandomNeuronForest(int n, float d)
{
	extern int NNEUR;
	FVector Pos;
	FRotator Rot;
	float R = 10.0f*d;// 300.0f;
	int nx = n;
	int ny = n;
	int nz = n;
	
	FVector DX,U, Ex, Ey, Ez;
	FVector CamPos = FVector(0, 0, 0);
	float dis,sz;
	int p = 0;
	Ex = J;// CAM_CY;
	Ey = K;// CAM_CZ;
	max_neuronas = (2 * nx + 1)*(2 * ny + 1)*(2 * nz + 1);
	Impostores = (ImpostorItem *)malloc(max_neuronas * sizeof(ImpostorItem));
		for (int k = -nz; k <= nz; k++) {
			for (int i = -nx; i <= nx; i++) {
				for (int j = -ny; j <= ny; j++) {
					Pos = FVector(R * i, R * j, R * k) + RandomVector(0.5f*R);
					sz = 100.0f*frand(0.8f, 1.2f);
					//Rot = RandomRotator(90.0f);
					DX = Pos - CamPos;
					dis = mod(DX);
					U = versor(DX);
					Pos = CamPos + U*dis;
					Impostores[p].Pos = Pos;
					Impostores[p].a = Pos - Ex*sz-Ey*sz;
					Impostores[p].b = Pos - Ex*sz+Ey*sz;
					Impostores[p].c = Pos + Ex*sz+Ey*sz;
					Impostores[p].d = Pos + Ex*sz-Ey*sz;
					p++;
					//BOSQUE_NEURONAS[NUM_NEURONAS++] = GWORLD->SpawnActor<ANeuron>(ANeuron::StaticClass(), Pos, Rot);
				}
			}
		}
	
}

void AImpostorNeuronForest::GenerateMesh()
{
	FCustomMeshTriangle tri;
	for (int i = 0; i < max_neuronas; i++) {
		//POR CADA ITEM GENERAMOS DOS TRIANGULOS
		tri.Vertex0 = Impostores[i].a;
		tri.Vertex1 = Impostores[i].b;
		tri.Vertex2 = Impostores[i].c;
		triangles.Add(tri);
		tri.Vertex0 = Impostores[i].a;
		tri.Vertex1 = Impostores[i].c;
		tri.Vertex2 = Impostores[i].d;
		triangles.Add(tri);
	}

}

void AImpostorNeuronForest::UpdateNeuronForest(FVector CamPos, FVector Cx, FVector Cy, float Fov)
{

}



