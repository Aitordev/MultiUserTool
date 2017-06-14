// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Neuron.h"
//#include "CodeGeneratedNeuron.h"
//#include "MyHUD.h"
//#include "MyPlayerController.h"
#include "MyPawn.generated.h"
/*
UCLASS()
class MULTIUSERTOOL_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	
	
};
*/


//class AMyPlayerController;
class UStaticMesh;
class ANeuron;
//class ACodeGeneratedNeuron;


		
UCLASS()
class MULTIUSERTOOL_API AMyPawn : public APawn
{
	GENERATED_BODY()
	//GENERATED_UCLASS_BODY()

public:
	//CONTROL DE LA CAMARA
	int CameraUpdateMode;
	FVector CameraAim;
	FVector CameraPos;
	FVector CameraUp;
	FVector Cx, Cy, Cz;
	float   CameraFOV;
	float CameraAzm, CameraElv, CameraDis;
	class UStaticMesh *Neuronas[32];
	int MaxNeuronas;

	//AMyPlayerController* MainPlayerController;
	// Sets default values for this pawn's properties
	AMyPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;


	UFUNCTION()
		void OnLBD();
	UFUNCTION()
		void OnLBU();
	UFUNCTION()
		void OnKBD();
	UFUNCTION()
		void OnKBU();



	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		class UStaticMeshComponent* NavigatorMesh;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
		USpringArmComponent* OurCameraSpringArm;
	/* Primary camera of the player*/
	UPROPERTY(VisibleAnywhere, Category = "Camera")
		UCameraComponent* OurCamera;
	//UPROPERTY(VisibleAnywhere, Category = "Camera")//DUDAS 
	UPROPERTY(EditAnywhere)
		USpotLightComponent* OurLight2;
	float UpdateFps(float DeltaSeconds);
	bool UpdateInput(float DeltaTime);
	bool EndInput();
	bool Pulsa(char c);
	bool KBD(char c);
	bool KBU(char c);
	bool LBD(void);
	void OnMouseWheel(float AxisValue);

	FVector Project(FVector &Xa);


	void UpdateCamera();
	bool UpdateInterface();
	void LoadNeuronForest(char* fich);
	bool FreeTheNeuronForest();
	bool FreeNeuronas();
	int LoadNeuronas(char *dir);
	ANeuron *AddNeurona();
	//ACodeGeneratedNeuron *AddCodeGeneratedNeurona();
	int SelectNeuron();

	void CreateRandomNeuronForest(int n, float d);
};
