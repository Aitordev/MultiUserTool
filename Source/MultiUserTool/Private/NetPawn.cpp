// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiUserTool.h"
#include "NetPawn.h"


// Sets default values
ANetPawn::ANetPawn()
{
	Role = ROLE_Authority;
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Take control of the default Player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	
	// Create a dummy root component we can attach things to.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	// Create a camera and a visible object
	UCameraComponent* OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
	OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
	// Attach our camera and visible object to our root component. Offset and rotate the camera.
	OurCamera->SetupAttachment(RootComponent);
	OurCamera->SetRelativeLocation(FVector(-250.0f, 0.0f, 250.0f));
	OurCamera->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	OurVisibleComponent->SetupAttachment(RootComponent);

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void ANetPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANetPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*
	if (GetWorld()) {
		//extern int NEURONS_TO_SPAW;
		APlayerController* MyController = GetWorld()->GetFirstPlayerController();
		MyController->GetMousePosition(GFMX, GFMY);
		MyController->bShowMouseCursor = true;
		MyController->bEnableClickEvents = true;
		MyController->bEnableMouseOverEvents = true;
	}
	*/
}

// Called to bind functionality to input
void ANetPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ANetPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ANetPawn::MoveRight);
	PlayerInputComponent->BindAxis("MoveTop", this, &ANetPawn::MoveTop);
	//PlayerInputComponent->BindAxis("CameraYaw", this, &ANetPawn::AddControllerYawInput);
	//PlayerInputComponent->BindAxis("CameraPitch", this, &ANetPawn::AddControllerPitchInput);
}

void ANetPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANetPawn, CurrentPosition);
	DOREPLIFETIME(ANetPawn, CurrentRotation);
}


void ANetPawn::OnRep_PosChange() {
	SetActorLocation(CurrentPosition);
}

void ANetPawn::OnRep_RotChange() {
	SetActorRotation(CurrentRotation);
}


void ANetPawn::MoveForward(float Value) {
	if (!Controller || Value == 0.0f) return;
	if (Role != ROLE_Authority && IsLocallyControlled()) {
		ServerMoveForward(Value, Controller->GetControlRotation());
	}
}

void ANetPawn::MoveRight(float Value) {
	if (!Controller || Value == 0.0f) return;
	if (Role != ROLE_Authority && IsLocallyControlled()) {
		ServerMoveRight(Value, Controller->GetControlRotation());
	}
}

void ANetPawn::MoveTop(float Value) {
	if (!Controller || Value == 0.0f) return;
	if (Role != ROLE_Authority && IsLocallyControlled()) {
		ServerMoveTop(Value, Controller->GetControlRotation());
	}
}

bool ANetPawn::ServerMoveForward_Validate(float Value, FRotator Rotation) {
	return true;
}

void ANetPawn::ServerMoveForward_Implementation(float Value, FRotator Rotation) {
	SetActorRotation(Rotation);
	CurrentRotation = Rotation;
	const FVector Direction = FRotationMatrix(CurrentRotation).GetScaledAxis(EAxis::X) * Value;
	SetActorLocation(GetActorLocation() + Direction);
	CurrentPosition = GetActorLocation();
}

bool ANetPawn::ServerMoveRight_Validate(float Value, FRotator Rotation) {
	return true;
}

void ANetPawn::ServerMoveRight_Implementation(float Value, FRotator Rotation) {
	SetActorRotation(Rotation);
	CurrentRotation = Rotation;
	const FVector Direction = FRotationMatrix(CurrentRotation).GetScaledAxis(EAxis::Y) * Value;
	SetActorLocation(GetActorLocation() + Direction);
	CurrentPosition = GetActorLocation();
}

bool ANetPawn::ServerMoveTop_Validate(float Value, FRotator Rotation) {
	return true;
}

void ANetPawn::ServerMoveTop_Implementation(float Value, FRotator Rotation) {
	SetActorRotation(Rotation);
	CurrentRotation = Rotation;
	const FVector Direction = FRotationMatrix(CurrentRotation).GetScaledAxis(EAxis::Z) * Value;
	SetActorLocation(GetActorLocation() + Direction);
	CurrentPosition = GetActorLocation();
}