// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiUserTool.h"
#include "MyDefaultPawn.h"

// Sets default values
AMyDefaultPawn::AMyDefaultPawn()
{
	CurrentFloatingName = "Name";

	FloatingName = CreateAbstractDefaultSubobject<UTextRenderComponent>(TEXT("FloatingName"));
	FloatingName->SetRelativeLocation(FVector(0,0,50));
	FloatingName->SetHorizontalAlignment(EHTA_Center);
	FloatingName->SetupAttachment(GetRootComponent());
	FloatingName->SetText(FText::FromString(CurrentFloatingName));

	bCanBeDamaged = false;
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MaxSpeed = 10.f;
	bReplicateMovement = true;
}

void AMyDefaultPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	MakeNameFacePlayer();
}

// Called to bind functionality to input
void AMyDefaultPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("DefaultPawn_MoveForward", this, &AMyDefaultPawn::MoveForward);
	PlayerInputComponent->BindAxis("DefaultPawn_MoveRight", this, &AMyDefaultPawn::MoveRight);
	PlayerInputComponent->BindAxis("DefaultPawn_MoveUp", this, &AMyDefaultPawn::MoveUp_World);
	
}

void AMyDefaultPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyDefaultPawn, CurrentPosition);
	DOREPLIFETIME(AMyDefaultPawn, CurrentRotation);
	DOREPLIFETIME(AMyDefaultPawn, CurrentFloatingName);
}


void AMyDefaultPawn::OnRep_PosChange() {
	SetActorLocation(CurrentPosition);
}

void AMyDefaultPawn::OnRep_RotChange() {
	SetActorRotation(CurrentRotation);
}

void AMyDefaultPawn::OnRep_CurrentFloatingName()
{
	FloatingName->SetText(FText::FromString(CurrentFloatingName));
}

void AMyDefaultPawn::MakeNameFacePlayer()
{
	AMyDefaultPawn* Character = Cast<AMyDefaultPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	FRotator NewRotation = Character->GetActorRotation();
	NewRotation.Yaw += 180.f;
	
	FloatingName->SetRelativeRotation(NewRotation);
}

void AMyDefaultPawn::MoveForward(float Value) {
	Super::MoveForward(Value);
	if (!Controller || Value == 0.0f) return;
	if (Role != ROLE_Authority && IsLocallyControlled()) {
		ServerMoveForward(Value, Controller->GetControlRotation());
	}
}

void AMyDefaultPawn::MoveRight(float Value) {
	Super::MoveRight(Value);
	if (!Controller || Value == 0.0f) return;
	if (Role != ROLE_Authority && IsLocallyControlled()) {
		ServerMoveRight(Value, Controller->GetControlRotation());
	}
}

void AMyDefaultPawn::MoveUp_World(float Value) {
	Super::MoveUp_World(Value);
	if (!Controller || Value == 0.0f) return;
	if (Role != ROLE_Authority && IsLocallyControlled()) {
		ServerMoveTop(Value, Controller->GetControlRotation());
	}
}

bool AMyDefaultPawn::SetCurrentFloatingName_Validate(const FString& Name)
{
	return true;
}

void AMyDefaultPawn::SetCurrentFloatingName_Implementation(const FString& Name)
{
	CurrentFloatingName = Name;
	FloatingName->SetText(FText::FromString(CurrentFloatingName));
}

bool AMyDefaultPawn::ServerMoveForward_Validate(float Value, FRotator Rotation) {
	return true;
}

void AMyDefaultPawn::ServerMoveForward_Implementation(float Value, FRotator Rotation) {
	SetActorRotation(Rotation);
	CurrentRotation = Rotation;
	const FVector Direction = FRotationMatrix(CurrentRotation).GetScaledAxis(EAxis::X) *Value * MaxSpeed;
	SetActorLocation(GetActorLocation() + Direction);
	CurrentPosition = GetActorLocation();
}

bool AMyDefaultPawn::ServerMoveRight_Validate(float Value, FRotator Rotation) {
	return true;
}

void AMyDefaultPawn::ServerMoveRight_Implementation(float Value, FRotator Rotation) {
	SetActorRotation(Rotation);
	CurrentRotation = Rotation;
	const FVector Direction = FRotationMatrix(CurrentRotation).GetScaledAxis(EAxis::Y) * Value * MaxSpeed;
	SetActorLocation(GetActorLocation() + Direction);
	CurrentPosition = GetActorLocation();
}

bool AMyDefaultPawn::ServerMoveTop_Validate(float Value, FRotator Rotation) {
	return true;
}

void AMyDefaultPawn::ServerMoveTop_Implementation(float Value, FRotator Rotation) {
	SetActorRotation(Rotation);
	CurrentRotation = Rotation;
	const FVector Direction = FRotationMatrix(CurrentRotation).GetScaledAxis(EAxis::Z) * Value * MaxSpeed;
	SetActorLocation(GetActorLocation() + Direction);
	CurrentPosition = GetActorLocation();
}

