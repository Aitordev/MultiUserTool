// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Net/UnrealNetwork.h"
#include "Neuron.h"
#include "Local.h"
#include "Navigator.generated.h"

/*
UCLASS()
class MULTIUSERTOOL_API ANavigator : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ANavigator();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	
	
};
*/

UCLASS()
class MULTIUSERTOOL_API ANavigator : public APawn
{
	GENERATED_UCLASS_BODY()

public:
	// Sets default values for this pawn's properties
	ANavigator();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	//MOVIMIENTOS DE TRANSLACION
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerMoveForward(float Value, FRotator Rotation);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerMoveRight(float Value, FRotator Rotation);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerMoveUpDown(float Value, FRotator Rotation);

	
	UPROPERTY(Transient, ReplicatedUsing = OnRep_PosChange)
	FVector CurrentPosition;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_RotChange)
	FRotator CurrentRotation;

	UFUNCTION()
	void OnRep_PosChange();

	UFUNCTION()
	void OnRep_RotChange();

protected:

	//UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UStaticMeshComponent* NavigatorMesh;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* OurCameraSpringArm;
	/* Primary camera of the player*/
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* OurCamera;
	//UPROPERTY(VisibleAnywhere, Category = "Camera")//DUDAS 

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* OurLightSpringArm;
	UPROPERTY(EditAnywhere)
	USpotLightComponent* OurLight2;

	UPROPERTY(EditAnywhere)
	UDirectionalLightComponent* DirLight;

/*
	UPROPERTY(EditAnywhere)
	USpringArmComponent* OurCameraSpringArm;
	UCameraComponent* OurCamera;
	USpotLightComponent* OurLight2;
*/
	//Input variables
	FVector MovementInput;
	FVector2D CameraInput;
	float ZoomFactor;
	float zoom;
	bool bZoomingIn;
	bool bZoomingOut;

	//POSICION DE LA CAMARA
	int CameraUpdateMode;
	FVector CameraAim;
	FVector CameraPos;
	FVector CameraUp;
	FVector Cx, Cy, Cz;
	float CameraAzm;
	float CameraElv;
	float CameraDis;
	float CameraFOV;
	float dt;


	//Input functions

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void MoveUpDown(float AxisValue);

	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);

	void WheelZoom(float AxisValue);
	void OnMouseWheel(float AxisValue);
	void ZoomIn();
	void ZoomOut();
	void EndZoomIn();
	void EndZoomOut();

	void ShowHide();

	float    UpdateFps(float DeltaSeconds);
	ANeuron *AddNeurona();

	void InicPosition();
	void UpdatePivotCamera();
	void UpdateFlightCamera();
	void UpdateCamera();
};
