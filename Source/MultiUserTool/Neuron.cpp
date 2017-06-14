// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiUserTool.h"
#include "XPlayerController.h"
#include "XPlayerState.h"
#include "Neuron.h"
class UMaterial;

UMaterial *TheMaterial = NULL;
UMaterial *TheNewMaterial = NULL;
UMaterial *TheEnhanceMaterial = NULL;

UMaterial *Materials[16];
int NumMaterials = 0;

UStaticMesh *NeuronMeshes[32];
int MaxNeuronMeshes = 0;

struct FConstructorNew {
	ConstructorHelpers::FObjectFinderOptional<UStaticMesh> newMesh;
	FConstructorNew(const TCHAR *name)
		: newMesh(name)
	{
	}
};


UStaticMesh *LoadMesh(const TCHAR *Name)
{
	FConstructorNew Constructor = FConstructorNew(Name);
	return Constructor.newMesh.Get();
}

UStaticMeshComponent *ANeuron::LoadNeuronMesh(const TCHAR * LocalName,const TCHAR * Name)
{
	int j = 0;
	//NeuronMeshes[j++] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NeuronMesh0"));
	//FConstructorNew Constructor= FConstructorNew(Name);
	UStaticMeshComponent *Mesh = CreateDefaultSubobject<UStaticMeshComponent>(LocalName);
	Mesh->SetStaticMesh(LoadMesh(Name));// Constructor.newMesh.Get());
	return Mesh;
}

int LoadNeuronMeshes()
{
	int j = 0;
	NeuronMeshes[j++] = LoadMesh(TEXT("/Game/Mesh/Neuron/neuron001.neuron001"));
	NeuronMeshes[j++] = LoadMesh(TEXT("/Game/Mesh/Neuron/neuron002.neuron002"));
	NeuronMeshes[j++] = LoadMesh(TEXT("/Game/Mesh/Neuron/neuron003.neuron003"));
	NeuronMeshes[j++] = LoadMesh(TEXT("/Game/Mesh/Neuron/neuron004.neuron004"));
	NeuronMeshes[j++] = LoadMesh(TEXT("/Game/Mesh/Neuron/neuron005.neuron005"));
	//NeuronMeshes[j++] = LoadMesh(TEXT("/Game/Mesh/Neuron/neuron006.neuron006"));
	MaxNeuronMeshes = j;
	return j;
}

UMaterial *LoadMaterial(const TCHAR *Name)
{
	static ConstructorHelpers::FObjectFinder<UMaterial> Material(Name);
	if (Material.Object != NULL) return (UMaterial*)Material.Object;
	return nullptr;
}

extern int MATERIAL_TYPE;

UMaterial * GetSelectedMaterial()
{
	static bool create = true;
	if (NumMaterials==0) {
		int j = 0;
		Materials[j++] = LoadMaterial(TEXT("Material'/Game/Material/neuron_mat.neuron_mat'"));
		Materials[j++] = LoadMaterial(TEXT("Material'/Game/Material/NewMaterial.NewMaterial'"));
		Materials[j++] = LoadMaterial(TEXT("Material'/Game/Material/NewMaterial2.NewMaterial2'"));
		Materials[j++] = LoadMaterial(TEXT("Material'/Game/Material/NewMaterial3.NewMaterial3'"));
		Materials[j++] = LoadMaterial(TEXT("Material'/Game/Material/NewMaterial4.NewMaterial4'"));
		Materials[j++] = LoadMaterial(TEXT("Material'/Game/Material/new_neuron_mat.new_neuron_mat'"));
		NumMaterials = j;
		create = false;
	}
	if (MATERIAL_TYPE < NumMaterials) return Materials[MATERIAL_TYPE];
	return Materials[0];
}

UMaterial * GetMaterial(int i)
{
	static bool create = true;
	if (NumMaterials == 0) {
		int j = 0;
		Materials[j++] = LoadMaterial(TEXT("Material'/Game/Material/neuron_mat.neuron_mat'"));
		Materials[j++] = LoadMaterial(TEXT("Material'/Game/Material/NewMaterial.NewMaterial'"));
		Materials[j++] = LoadMaterial(TEXT("Material'/Game/Material/NewMaterial2.NewMaterial2'"));
		Materials[j++] = LoadMaterial(TEXT("Material'/Game/Material/NewMaterial3.NewMaterial3'"));
		Materials[j++] = LoadMaterial(TEXT("Material'/Game/Material/NewMaterial4.NewMaterial4'"));
		Materials[j++] = LoadMaterial(TEXT("Material'/Game/Material/new_neuron_mat.new_neuron_mat'"));
		NumMaterials = j;
		create = false;
	}
	if (i < NumMaterials) return Materials[i];
	return Materials[0];
}




int MESH_TYPE = 1;

// Sets default values
ANeuron::ANeuron()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
	PrimaryActorTick.bCanEverTick = true;
	SetActorEnableCollision(true);

	idneuron = FMath::RandRange(0, 99999);

	SetReplicates(true);
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateMovement = true;

	// Structure to hold one-time initialization
	/*
	struct FConstructorStatics {
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> neuronMesh;
		FConstructorStatics()
			//: neuronMesh(TEXT("/Game/Mesh/Neuron/Neurona_sm.Neurona_sm"))
			//StaticMesh'/Game/Mesh/Neuron/Inter_000_L0.Inter_000_L0'
			//: neuronMesh(TEXT("/Game/Mesh/Neuron/Inter_000_L0_SM.Inter_000_L0_SM"))
			: neuronMesh(TEXT("/Game/Mesh/Neuron/neuron005.neuron005"))
			//: neuronMesh(TEXT("/Game/Mesh/Neuron/Inter_000_L0.Inter_000_L0"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;
	
	// Create static mesh component
	neuronMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NeuronMesh0"));
	neuronMesh->SetStaticMesh(ConstructorStatics.neuronMesh.Get());
	*/
	switch (MESH_TYPE) {
		case 1:
			neuronMesh = LoadNeuronMesh(TEXT("NeuronMesh1"), TEXT("/Game/Mesh/Neuron/neuron001.neuron001"));
		break;
		case 2:
			neuronMesh = LoadNeuronMesh(TEXT("NeuronMesh2"), TEXT("/Game/Mesh/Neuron/neuron002.neuron002"));
			break;
		case 3:
			neuronMesh = LoadNeuronMesh(TEXT("NeuronMesh3"), TEXT("/Game/Mesh/Neuron/neuron003.neuron003"));
			break;
		case 4:
			neuronMesh = LoadNeuronMesh(TEXT("NeuronMesh4"), TEXT("/Game/Mesh/Neuron/neuron004.neuron004"));
			break;
		case 5:
			neuronMesh = LoadNeuronMesh(TEXT("NeuronMesh5"), TEXT("/Game/Mesh/Neuron/neuron005.neuron005"));
		break;
		case 6:
			neuronMesh = LoadNeuronMesh(TEXT("NeuronMesh6"), TEXT("/Game/Mesh/Neuron/neuron006.neuron006"));
		break;
	}

	Materials.Add(LoadMaterial(TEXT("Material'/Game/Material/new_neuron_mat.new_neuron_mat'")));
	Materials.Add(LoadMaterial(TEXT("Material'/Game/Material/neuron_mat.neuron_mat'")));
	Materials.Add(LoadMaterial(TEXT("Material'/Game/Material/NewMaterial2.NewMaterial2'")));
	Materials.Add(LoadMaterial(TEXT("Material'/Game/Material/NewMaterial3.NewMaterial3'")));
	Materials.Add(LoadMaterial(TEXT("Material'/Game/Material/NewMaterial4.NewMaterial4'")));
	NumMaterials = 4;
	//UStaticMesh *mesh = LoadMesh(TEXT("/Game/Mesh/Neuron/neuron001.neuron001"));
	//neuronMesh->SetStaticMesh(LoadMesh(TEXT("/Game/Mesh/Neuron/neuron005.neuron005")));

	NewMaterial = nullptr;

	//Asignar un material a la neurona
	if (!TheMaterial) {
		static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/Material/neuron_mat.neuron_mat'"));
		if (Material.Object != NULL) {
			TheMaterial = (UMaterial*)Material.Object;
		}
	}
	if (!TheNewMaterial) {
		static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/Material/new_neuron_mat.new_neuron_mat'"));
		if (Material.Object != NULL) {
			TheNewMaterial = (UMaterial*)Material.Object;
		}
	}
	if (!TheEnhanceMaterial) {
		static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/Material/NewMaterial3.NewMaterial3'"));
		if (Material.Object != NULL) {
			TheEnhanceMaterial = (UMaterial*)Material.Object;
		}
	}

	neuronMesh->SetMobility(EComponentMobility::Movable);
	//neuronMesh->SetMaterial(0, TheMaterial);
	neuronMesh->SetMaterial(0, TheNewMaterial);
	//neuronMesh->SetMaterial(0, TheEnhanceMaterial);
	
	//neuronMesh->SetMaterial(0,GetMaterial(0));

	RootComponent = neuronMesh;
	//SetReplicates(true);
	//SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	
	//mouseover
	neuronMesh->OnBeginCursorOver.AddDynamic(this, &ANeuron::CustomOnBeginMouseOver);
	//this->OnActorHit.AddDynamic(this, &ANeuron::OnHit);
	//neuronMesh->OnComponentHit.AddDynamic(this, &ANeuron::OnHit);
	
}

void ANeuron::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANeuron, isEnhaced);
	DOREPLIFETIME(ANeuron, neuronMesh);
	DOREPLIFETIME(ANeuron, idneuron);
	
}

// Called when the game starts or when spawned
void ANeuron::BeginPlay()
{
	Super::BeginPlay();

}

void ANeuron::SetMaterial(UMaterial *material)
{
	/*
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(neuronMesh->GetMaterial(0), this);
	// Change color to yellow 
	DynamicMaterial->SetVectorParameterValue("BodyColor", FLinearColor::Yellow);
	// Set the mesh's material to the new one 
	neuronMesh->SetMaterial(0, DynamicMaterial);
	*/
	NewMaterial = material;
	//neuronMesh->SetMaterial(0,material);
}

void ANeuron::SetMaterial(int i)
{
	if (i < NumMaterials) {
		neuronMesh->SetMaterial(0, TheEnhanceMaterial);// Materials[i]);
	}
}

// Called every frame
void ANeuron::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UpdateFps(DeltaSeconds);
	if (NewMaterial) {
		neuronMesh->SetMaterial(0,NewMaterial);
		NewMaterial = nullptr;
	}

}

void ANeuron::CustomOnBeginMouseOver(UPrimitiveComponent* TouchedComponent)
{
	AXPlayerController* MyPC = Cast<AXPlayerController>(GetWorld()->GetFirstPlayerController());
	AXPlayerState* MyPS = Cast<AXPlayerState>(MyPC->PlayerState);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("is Enhaced %s"), isEnhaced? TEXT("true") : TEXT("false")));
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("Name %s"), *MyPS->PlayerName));
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("id %d"), idneuron));
	}
	//MyPC->ToggleNeuronWidget();
	MyPC->EnhanceNeuron(idneuron);
	//ANeuron::Enhance(!isEnhaced);// toggle		
}

bool ANeuron::Enhance_Validate()
{
	return true;
}

void ANeuron::Enhance_Implementation()
{
	if (!isEnhaced) {
		neuronMesh->SetMaterial(0, TheEnhanceMaterial);
		isEnhaced = true;
		EnhanceMultiCast(isEnhaced);
	}
	else {
		neuronMesh->SetMaterial(0, TheMaterial);
		isEnhaced = false;
		EnhanceMultiCast(isEnhaced);
	}
	/*if (enhace) {
		neuronMesh->SetMaterial(0, TheEnhanceMaterial);
		isEnhaced = true;
		EnhanceMultiCast(isEnhaced);

	}
	else {
		neuronMesh->SetMaterial(0, TheMaterial);
		isEnhaced = false;
		EnhanceMultiCast(isEnhaced);
	}*/
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("server is Enhaced %s"), isEnhaced ? TEXT("true") : TEXT("false")));
}

bool ANeuron::EnhanceMultiCast_Validate(bool enhace)
{
	return true;
}

void ANeuron::EnhanceMultiCast_Implementation(bool enhace)
{
	if (enhace) {
		neuronMesh->SetMaterial(0, TheEnhanceMaterial);
		isEnhaced = true;
	}
	else {
		neuronMesh->SetMaterial(0, TheMaterial);
		isEnhaced = false;
	}
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("multicast is Enhaced %s"), isEnhaced ? TEXT("true") : TEXT("false")));
}


