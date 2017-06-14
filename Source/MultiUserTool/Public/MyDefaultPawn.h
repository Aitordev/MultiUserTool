// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/DefaultPawn.h"
#include "MyDefaultPawn.generated.h"

/**
 * 
 */
UCLASS()
class MULTIUSERTOOL_API AMyDefaultPawn : public ADefaultPawn
{
	GENERATED_BODY()

public:

	class UTextRenderComponent* FloatingName;

	// Sets default values for this pawn's properties
	AMyDefaultPawn();
	
	virtual void Tick( float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	float MaxSpeed;

	float Acceleration;

	float Deceleration;

	virtual void MoveForward(float Value) override;

	virtual void MoveRight(float Value) override;

	virtual void MoveUp_World(float Val) override;

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerMoveForward(float Value, FRotator Rotation);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerMoveRight(float Value, FRotator Rotation);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerMoveTop(float Value, FRotator Rotation);

	UFUNCTION(Server, Reliable, WithValidation)
		void SetCurrentFloatingName(const FString& Name);

	UPROPERTY(Transient, ReplicatedUsing = OnRep_PosChange)
		FVector CurrentPosition;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_RotChange)
		FRotator CurrentRotation;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Transient, ReplicatedUsing = OnRep_CurrentFloatingName)
		FString CurrentFloatingName;

	UFUNCTION()
		void OnRep_PosChange();

	UFUNCTION()
		void OnRep_RotChange();
	
	UFUNCTION()
		void OnRep_CurrentFloatingName();

	void MakeNameFacePlayer();
};
