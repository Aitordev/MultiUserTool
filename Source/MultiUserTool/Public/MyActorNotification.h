// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MyActorNotification.generated.h"

UCLASS()
class MULTIUSERTOOL_API AMyActorNotification : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActorNotification();

	/** Name of the MeshComponent. Use this name if you want to prevent creation of the component (with ObjectInitializer.DoNotCreateDefaultSubobject). */
	static FName MeshComponentName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = NeuronUi)
	int32 idneuron;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = NeuronUi)
	int32 idInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = NeuronUi)
	class UStaticMeshComponent* sphereMesh;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Replicated, Category = NeuronUi)
	FString NeuronInfo;

	//Function to handle the interaction
	UFUNCTION(BlueprintCallable)
	void CustomOnBeginMouseOver(UPrimitiveComponent* TouchedComponent);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
