// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Net/UnrealNetwork.h"
#include "NetPawn.generated.h"

UCLASS()
class MULTIUSERTOOL_API ANetPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ANetPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void MoveForward(float Value);

	UFUNCTION()
	void MoveRight(float Value);
	
	UFUNCTION()
	void MoveTop(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerMoveForward(float Value, FRotator Rotation);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerMoveRight(float Value, FRotator Rotation);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerMoveTop(float Value, FRotator Rotation);

	UPROPERTY(Transient, ReplicatedUsing = OnRep_PosChange)
	FVector CurrentPosition;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_RotChange)
	FRotator CurrentRotation;

	UPROPERTY(EditAnywhere)
	USceneComponent* OurVisibleComponent;

	UFUNCTION()
	void OnRep_PosChange();

	UFUNCTION()
	void OnRep_RotChange();
	
};
