// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Neuron.generated.h"

UCLASS()
class MULTIUSERTOOL_API ANeuron : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANeuron();
	//Para el material
	UPROPERTY(VisibleAnywhere, Replicated, Category = Mesh)
	class UStaticMeshComponent* neuronMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = NeuronUi)
		int32 idneuron;

	//Function to handle the interaction
	UFUNCTION()
	void CustomOnBeginMouseOver(UPrimitiveComponent* TouchedComponent);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//UPROPERTY(Transient, ReplicatedUsing = OnRep_IsEnhaced)
	UPROPERTY(VisibleAnywhere, Replicated)
	bool isEnhaced = false;

	UFUNCTION(Server, Reliable, WithValidation)
	void Enhance();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	// then the server calls the function with a multicast that executes on all clients and the server
	virtual void EnhanceMultiCast(bool enhace);

	//int LoadNeuronMeshes();
	//int LoadNeuronMesh(FString Name);
	//UStaticMeshComponent * LoadNeuronMesh(const TCHAR * Name);
	UStaticMeshComponent *ANeuron::LoadNeuronMesh(const TCHAR * LocalName, const TCHAR * Name);

	void SetMaterial(UMaterial *material);

	UMaterial *NewMaterial;
	UPROPERTY(EditAnywhere)
	TArray<UMaterialInterface*> Materials;
	int NumMaterials;

	void SetMaterial(int i);
	
};
