// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiUserTool.h"
#include "XPlayerController.h"
#include "MyActorNotification.h"


FName AMyActorNotification::MeshComponentName(TEXT("MeshComponent0"));

// Sets default values
AMyActorNotification::AMyActorNotification()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	idInfo = FMath::RandRange(0, 99999);
	NeuronInfo = TEXT("No info set.");

	bReplicates = true;
	bAlwaysRelevant = true;

	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh;
		FConstructorStatics()
	//		: SphereMesh(TEXT("/Engine/EngineMeshes/Sphere")) {}
			: SphereMesh(TEXT("/Game/Sprites/EditorSphere2")) {}
	};

	static FConstructorStatics ConstructorStatics;

	sphereMesh = CreateOptionalDefaultSubobject<UStaticMeshComponent>(ADefaultPawn::MeshComponentName);
	sphereMesh->SetStaticMesh(ConstructorStatics.SphereMesh.Object);
		RootComponent = sphereMesh;
	

	sphereMesh->OnBeginCursorOver.AddDynamic(this, &AMyActorNotification::CustomOnBeginMouseOver);

}


void AMyActorNotification::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AMyActorNotification, isEnhaced);
	DOREPLIFETIME(AMyActorNotification, idneuron);
	DOREPLIFETIME(AMyActorNotification, idInfo);
	DOREPLIFETIME(AMyActorNotification, NeuronInfo);

}

// Called when the game starts or when spawned
void AMyActorNotification::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyActorNotification::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyActorNotification::CustomOnBeginMouseOver(UPrimitiveComponent* TouchedComponent)
{
	AXPlayerController* MyPC = Cast<AXPlayerController>(GetWorld()->GetFirstPlayerController());
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("AMyActorNotification id %d"), idneuron));
	}
	MyPC->ToggleNeuronInfoWidget(idneuron, idInfo, NeuronInfo);
	//MyPC->EnhanceNeuron(idneuron);
}

