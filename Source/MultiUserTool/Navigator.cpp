// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiUserTool.h"
#include "Funciones.h"
#include "Local.h"
#include "LightControls.h"
#include "Navigator.h"


/*
// Sets default values
ANavigator::ANavigator()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANavigator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANavigator::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void ANavigator::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}
*/

//NUEVO CODIGO
extern int SELECTED_CONTROL;
extern bool TRACKING_SLIDER = false;

// Sets default values
//ANavigator::ANavigator(const class FPostConstructInitializeProperties& PCIP): super(PCIP)
ANavigator::ANavigator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Role = ROLE_Authority;
	
	struct FConstructorStatics {
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> NavigatorMesh;
		FConstructorStatics()
			: NavigatorMesh(TEXT("/Game/Mesh/eyeball.eyeball"))
		{
		}
	};
	
	
	static FConstructorStatics ConstructorStatics;
	// Create static mesh component
	//NavigatorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NavigatorMesh0"));
	NavigatorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NavigatorMesh0"));
	NavigatorMesh->SetStaticMesh(ConstructorStatics.NavigatorMesh.Get());
	//RootComponent = NavigatorMesh;
	/*

	// Create and position a mesh component so we can see where our sphere is
	//UStaticMeshComponent* SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	NavigatorMesh= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	NavigatorMesh->AttachTo(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("jabon.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		NavigatorMesh->SetStaticMesh(SphereVisualAsset.Object);
		NavigatorMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
		NavigatorMesh->SetWorldScale3D(FVector(0.8f));
	}
	*/
	InicPosition();
	/*
	SetActorLocation(FVector(0.0f, 0.0f, 0.0f));
	//bAddDefaultMovementBinding = false;

	//Create our components
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	NavigatorMesh->AttachTo(RootComponent);
	NavigatorMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	NavigatorMesh->SetWorldScale3D(FVector(0.05f));

	OurCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	OurCameraSpringArm->AttachTo(RootComponent);
	OurCameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 250.0f), FRotator(-60.0f, 0.0f, 0.0f));
	OurCameraSpringArm->TargetArmLength = 0.0f;
	OurCameraSpringArm->bEnableCameraLag = false;// true;
	OurCameraSpringArm->CameraLagSpeed = 3.0f;

	OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	OurCamera->AttachTo(OurCameraSpringArm, USpringArmComponent::SocketName);

	OurLight2 = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLightComp"));
	OurLight2->AttachTo(OurCameraSpringArm, TEXT("LightSocket"), EAttachLocation::SnapToTarget);
	OurLight2->AddLocalRotation(FRotator(0, 0, 0));
	OurLight2->AddLocalOffset(FVector(-200.0f, 0.0f, 0.0f));

	OurLight2->SetOuterConeAngle(90.0f);
	OurLight2->SetLightColor(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
	OurLight2->SetAttenuationRadius(10000.0f);
	*/
	zoom = 0.0f;

	//Take control of the default Player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	SetReplicates(true);
	//bUseControllerRotationPitch = true;
	//bUseControllerRotationYaw = true;
	bReplicates = true;
}

void ANavigator::OnRep_PosChange() {
	SetActorLocation(CurrentPosition);
}

void ANavigator::OnRep_RotChange() {
	SetActorRotation(CurrentRotation);
}

bool ANavigator::ServerMoveForward_Validate(float Value, FRotator Rotation) {
	return true;
}

void ANavigator::ServerMoveForward_Implementation(float Value, FRotator Rotation) {
	SetActorRotation(Rotation);
	CurrentRotation = Rotation;
	const FVector Direction = FRotationMatrix(CurrentRotation).GetScaledAxis(EAxis::X) * Value;
	SetActorLocation(GetActorLocation() + Direction);
	CurrentPosition = GetActorLocation();
}

bool ANavigator::ServerMoveRight_Validate(float Value, FRotator Rotation) {
	return true;
}

void ANavigator::ServerMoveRight_Implementation(float Value, FRotator Rotation) {
	SetActorRotation(Rotation);
	CurrentRotation = Rotation;
	const FVector Direction = FRotationMatrix(CurrentRotation).GetScaledAxis(EAxis::Y) * Value;
	SetActorLocation(GetActorLocation() + Direction);
	CurrentPosition = GetActorLocation();
}

bool ANavigator::ServerMoveUpDown_Validate(float Value, FRotator Rotation) {
	return true;
}

void ANavigator::ServerMoveUpDown_Implementation(float Value, FRotator Rotation) {
	SetActorRotation(Rotation);
	CurrentRotation = Rotation;
	const FVector Direction = FRotationMatrix(CurrentRotation).GetScaledAxis(EAxis::Z) * Value;
	SetActorLocation(GetActorLocation() + Direction);
	CurrentPosition = GetActorLocation();
}

// Called when the game starts or when spawned
void ANavigator::BeginPlay()
{
	Super::BeginPlay();

}

float ANavigator::UpdateFps(float DeltaSeconds)
{
	extern float FPS;
	static int frames = 0;
	static float Ti = 0.0f;
	static float T = 0.0f;
	T += DeltaSeconds;
	frames++;
	if (T > 1.0f) {
		FPS = (float)frames / T;
		T = 0.0f;
		frames = 0;
	}
	return FPS;
}
//extern float GZOOM;
// Called every frame

#define CAM_PIVOT  0
#define CAM_FLIGHT 1
int NAVIGATION_MODE = CAM_PIVOT;

void ANavigator::UpdatePivotCamera()
{
	
	CameraUp = K;
	if (MAX_NEURONAS) {
		//CameraAim = NEURONS_POS[MAX_NEURONAS - 1];// FVector(0.0f, 0.0f, 0.0f);
		CameraAim = FVector(0.0f, 0.0f, 0.0f);
	}
	else {
		CameraAim = FVector(0.0f, 0.0f, 0.0f);
	}
	GFOV = 10.0f / GZOOM;
	CameraAzm = AZM;
	CameraElv = ELV;
	CameraFOV = GFOV;
	//CameraAzm = 45.0f;
	//CameraElv = 45.0f;
	CameraDis = DIS = 1500.0f;
	CameraPos = CameraAim + versor(CameraAzm, CameraElv)*CameraDis;
	Cx = versor(CameraAim - CameraPos);
	Cy = -versor(cross(CameraUp, Cx));
	Cz = cross(Cx, Cy);


	//AHORA COLOCAMOS LA CAMARA
	FTransform T = FTransform(Cx, Cy, Cz, CameraPos);
	FQuat Q = T.GetRotation();
	FRotator R = FRotator(Q);
	FRotator Rot1 = FRotator(0.0f, 0.0f, 180.0f + CameraAzm);// T.Rotator();
	FRotator Rot2 = FRotator(-CameraElv, 0.0f, 0.0f);
	FRotator Rot = FRotator(-CameraElv, 180.0f + CameraAzm, 0.0f);
	if (1) {
		OurCameraSpringArm->SetRelativeLocationAndRotation(CameraPos, Rot);// FRotator(-60.0f, 0.0f, 0.0f));
																		   //OurCameraSpringArm->AddRelativeRotation(Rot1);//OurCameraSpringArm->SetRelativeLocationAndRotation(CameraPos, Q);

																		   //OurCameraSpringArm->SetRelativeTransform(T);
		OurCamera->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
	}
	else {
		OurCameraSpringArm->SetRelativeLocationAndRotation(CameraPos, Rot1);// FRotator(-60.0f, 0.0f, 0.0f));
		OurCamera->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), Rot2);//FRotator(0.0f, 0.0f, 0.0f));
	}

	OurCamera->FieldOfView = CameraFOV;

	/*EJES PARA PROYECCION
	Cz = versor(CameraAim - CameraPos);
	Cy = versor(cross(Cz,CameraUp));
	Cx = cross(Cz, Cy);
	*/
}

void ANavigator::InicPosition()
{
	CameraUpdateMode= CAM_PIVOT;
	CameraAzm=0.0f;
	CameraElv=0.0f;
	CameraDis=1500.0f;
	CameraFOV=60.0f;
	dt=0.016f;
	CameraDis = DIS = 1500.0f;
	CameraPos = CameraAim + versor(CameraAzm, CameraElv)*CameraDis;
	Cx = versor(CameraAim - CameraPos);
	Cy = -versor(cross(CameraUp, Cx));
	Cz = cross(Cx, Cy);
	
	
	SetActorLocation(FVector(0.0f, 0.0f, 0.0f));
	//bAddDefaultMovementBinding = false;

	//Create our components
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	if (NavigatorMesh) {
		NavigatorMesh->AttachTo(RootComponent);
		NavigatorMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		NavigatorMesh->SetWorldScale3D(FVector(0.0f));
	}

	OurCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	OurCameraSpringArm->AttachTo(RootComponent);
	OurCameraSpringArm->SetRelativeLocationAndRotation(CameraPos, FRotator(0.0f, 0.0f, 180.0f));
	//OurCameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 250.0f), FRotator(-60.0f, 0.0f, 0.0f));
	OurCameraSpringArm->TargetArmLength = CameraDis;
	OurCameraSpringArm->bEnableCameraLag = false;// true;
	OurCameraSpringArm->CameraLagSpeed = 3.0f;

	OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	OurCamera->AttachTo(OurCameraSpringArm, USpringArmComponent::SocketName);
	OurCamera->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 180.0f, 0.0f));
	OurCamera->ProjectionMode = ECameraProjectionMode::Perspective;
	OurCamera->FieldOfView = GFOV= CameraFOV;

	OurLightSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("LightSpringArm"));
	OurLightSpringArm->AttachTo(RootComponent);
	OurLightSpringArm->SetRelativeLocationAndRotation(CameraPos, FRotator(0.0f, 0.0f, 180.0f));
	//OurCameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 250.0f), FRotator(-60.0f, 0.0f, 0.0f));
	OurLightSpringArm->TargetArmLength = CameraDis;
	OurLightSpringArm->bEnableCameraLag = false;// true;
	OurLightSpringArm->CameraLagSpeed = 0.0f;


	OurLight2 = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLightComp"));
	OurLight2->AttachTo(OurLightSpringArm, TEXT("LightSocket"), EAttachLocation::SnapToTarget);
	OurLight2->AddLocalRotation(FRotator(0, 0, 0));
	OurLight2->AddLocalOffset(FVector(-200.0f, 0.0f, 0.0f));

	OurLight2->SetOuterConeAngle(90.0f);
	OurLight2->SetLightColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	OurLight2->SetAttenuationRadius(10000.0f);
	OurLight2->SetMobility(EComponentMobility::Movable);

	/*
	DirLight= CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("DisLightComp"));
	DirLight->SetIntensity(1.0f);
	DirLight->SetLightColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	DirLight->SetMobility(EComponentMobility::Movable);
	FHitResult SweepHitResult;
	DirLight->K2_SetWorldRotation(FRotator(0,0, 0), false, SweepHitResult, false);// ETeleportType::None);
	*/
}


void ANavigator::UpdateFlightCamera()
{
	//Zoom in if ZoomIn button is down, zoom back out if it's not
	{
		if (bZoomingIn)
		{
			ZoomFactor +=  0.1f*dt;         //Zoom in over half a second
		}
		if (bZoomingOut)
		{
			ZoomFactor -= 0.25f*dt;        //Zoom out over a quarter of a second
		}
		GZOOM = ZoomFactor;
		//ZoomFactor = sin(t);
		//ZoomFactor = FMath::Clamp<float>(ZoomFactor, 0.0f, 1.0f);//
		//ZoomFactor = zoom;

		//Blend our camera's FOV and our SpringArm's length based on ZoomFactor
		OurCamera->FieldOfView = FMath::Lerp<float>(120.0f, 5.0f, ZoomFactor);
		OurCameraSpringArm->TargetArmLength = FMath::Lerp<float>(400.0f, 300.0f, ZoomFactor);
		//bZoomingIn = bZoomingOut = false;
		//GZOOM = ZoomFactor;
	}

	//Rotate our actor's yaw, which will turn our camera because we're attached to it
	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += CameraInput.X;
		SetActorRotation(NewRotation);
	}

	//Rotate our camera's pitch, but limit it so we're always looking downward
	if (1) {
		FRotator NewRotation = OurCameraSpringArm->GetComponentRotation();
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraInput.Y, -80.0f, 15.0f);
		OurCameraSpringArm->SetWorldRotation(NewRotation);
	}

	//Handle movement based on our "MoveX" and "MoveY" axes
	
	if (!MovementInput.IsZero())
	{
		//Scale our movement input axis values by 100 units per second
		MovementInput = MovementInput.SafeNormal() * 100.0f;
		FVector NewLocation = GetActorLocation();
		NewLocation += GetActorForwardVector() * MovementInput.X * dt;
		NewLocation += GetActorRightVector() * MovementInput.Y * dt;
		NewLocation += GetActorUpVector() * MovementInput.Z * dt;
		SetActorLocation(NewLocation);
	}

}

extern float LIGHT_ELV, LIGHT_AZM;
bool HIDE_NAVIGATOR=false;

void ANavigator::ShowHide()
{
	static bool show_hide = HIDE_NAVIGATOR;
	if (NavigatorMesh && show_hide!= HIDE_NAVIGATOR) {
		if (HIDE_NAVIGATOR) NavigatorMesh->SetWorldScale3D(FVector(0.0f)); else NavigatorMesh->SetWorldScale3D(FVector(0.05f));
		show_hide = HIDE_NAVIGATOR;
	}
}

extern float LIGHT_INT;
extern float LIGHT_ANG;
extern float LIGHT_ATN;
extern float LIGHT_RED;
extern float LIGHT_GREEN;
extern float LIGHT_BLUE;

void ANavigator::UpdateCamera()
{
	extern int SELECTED_CONTROL;
	if (!SELECTED_CONTROL && !TRACKING_SLIDER) {
		switch (CameraUpdateMode) {
			case CAM_PIVOT:
				UpdatePivotCamera();
				break;
			case CAM_FLIGHT:
				UpdateFlightCamera();
				break;
		}
		CAM_POSITION = CameraPos;
		CAM_CX = Cx;
		CAM_CY = Cy;
		CAM_CZ = Cz;
		CAM_FOV = CameraFOV;
	}

	FHitResult SweepHitResult;
	OurLight2->K2_SetRelativeRotation(FRotator(LIGHT_ELV, LIGHT_AZM, 0) ,false,SweepHitResult, false);

	//OurLight2->AddLocalRotation(FRotator(LIGHT_ELV, LIGHT_AZM, 0));
	//OurLight2->AddLocalOffset(FVector(-200.0f, 0.0f, 0.0f));

	OurLight2->SetOuterConeAngle(LIGHT_ANG);
	OurLight2->SetLightColor(FLinearColor(LIGHT_RED, LIGHT_GREEN, LIGHT_BLUE, 1.0f));
	OurLight2->SetAttenuationRadius(LIGHT_ATN);
}


void ANavigator::Tick(float DeltaTime)
{
	//static bool inicWorld = true;
	Super::Tick(DeltaTime);
	UpdateFps(DeltaTime);
	dt = GDT=DeltaTime;
	if (GetWorld()) {
		//extern int NEURONS_TO_SPAW;
		APlayerController* MyController = GetWorld()->GetFirstPlayerController();
		MyController->GetMousePosition(GFMX, GFMY);
		MyController->bShowMouseCursor = true;
		MyController->bEnableClickEvents = true;
		MyController->bEnableMouseOverEvents = true;
	}
	//DEBERIAMOS TOMAR LOS MOVIMIENTOS DEL HUD
	//if(SELECTED_CONTROL==0) 
	UpdateCamera();
	ShowHide();
	
}

ANeuron *ANavigator::AddNeurona()
{
	float R = 500.0f;
	FVector Pos = RandomVector(R);
	FRotator Rot = RandomRotator(90.0f);
	ANeuron *neurona = GetWorld()->SpawnActor<ANeuron>(ANeuron::StaticClass(), Pos, Rot);
	NEURONAS[MAX_NEURONAS++] = new NEURON_REF(neurona, Pos, Rot);
	return neurona;
}

// Called to bind functionality to input
void ANavigator::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAxis("WheelZoom", this, &ANavigator::WheelZoom);
	
	//Hook up events for "ZoomIn"
	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &ANavigator::ZoomIn);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &ANavigator::ZoomOut);
	InputComponent->BindAction("EndZoomIn", IE_Released, this, &ANavigator::EndZoomIn);
	InputComponent->BindAction("EndZoomOut", IE_Released, this, &ANavigator::EndZoomOut);

	//Hook up every-frame handling for our four axes
	InputComponent->BindAxis("MoveForward", this, &ANavigator::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ANavigator::MoveRight);
	InputComponent->BindAxis("MoveTop", this, &ANavigator::MoveUpDown);
	InputComponent->BindAxis("CameraPitch", this, &ANavigator::AddControllerPitchInput);// PitchCamera);
	InputComponent->BindAxis("CameraYaw", this, &ANavigator::AddControllerYawInput);// YawCamera);
	
	

}


//Input functions
/*void ANavigator::MoveForward(float AxisValue)
{
	MovementInput.X = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}*/


/*void ANavigator::MouseMove(float AxisValue)
{
	int mx=MouseX;
	my=MouseY;
	UpdateControls();

MovementInput.X = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}*/


void ANavigator::MoveForward(float Value) {
	if (CameraUpdateMode==CAM_PIVOT || !Controller || Value == 0.0f) return;
	if (Role != ROLE_Authority && IsLocallyControlled()) {
		ServerMoveForward(Value*50.0f, Controller->GetControlRotation());
	}
}

void ANavigator::MoveRight(float Value)
{
	//MovementInput.Y = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
	if (CameraUpdateMode == CAM_PIVOT || !Controller || Value == 0.0f) return;
	if (Role != ROLE_Authority && IsLocallyControlled()) {
		ServerMoveRight(Value*50.0f, Controller->GetControlRotation());
	}
}

void ANavigator::MoveUpDown(float Value)
{
	//MovementInput.Z = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
	if (CameraUpdateMode == CAM_PIVOT || !Controller || Value == 0.0f) return;
	if (Role != ROLE_Authority && IsLocallyControlled()) {
		ServerMoveUpDown(Value*50.0f, Controller->GetControlRotation());
	}
}




/*
void ANavigator::MoveTop(float AxisValue)
{
	MovementInput.Z = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}
*/

void ANavigator::PitchCamera(float AxisValue)
{
	if (!TRACKING_SLIDER) {
		if (CameraUpdateMode == CAM_FLIGHT) CameraInput.Y = AxisValue;
	}
}

void ANavigator::YawCamera(float AxisValue)
{
	if (!TRACKING_SLIDER) {
		if (CameraUpdateMode == CAM_FLIGHT) CameraInput.X = AxisValue;
	}
}

void ANavigator::WheelZoom(float AxisValue)
{
	if (CameraUpdateMode == CAM_FLIGHT) {
		zoom += 0.01f*AxisValue;
		if (zoom < 0.0f) zoom = 0.0f;
		if (zoom > 1.0f) zoom = 1.0f;
	}
	else {
		GZOOM += 0.05f*AxisValue;
		if (GZOOM < 0.0f) GZOOM = 0.0f;
		if (GZOOM > 10.0f) GZOOM = 10.0f;
		GMWHEEL = AxisValue;
	}
}

void ANavigator::OnMouseWheel(float AxisValue)
{
	if (CameraUpdateMode == CAM_PIVOT) {
		GZOOM += 0.05f*AxisValue;
		if (GZOOM < 0.0f) GZOOM = 0.0f;
		if (GZOOM > 10.0f) GZOOM = 10.0f;
		GMWHEEL = AxisValue;
	}
}

void ANavigator::ZoomIn()
{
	if (CameraUpdateMode == CAM_FLIGHT) bZoomingIn = true;
}

void ANavigator::ZoomOut()
{
	if (CameraUpdateMode == CAM_FLIGHT) bZoomingOut = true;
}

void ANavigator::EndZoomIn()
{
	if (CameraUpdateMode == CAM_FLIGHT) bZoomingIn = false;
}

void ANavigator::EndZoomOut()
{
	if (CameraUpdateMode == CAM_FLIGHT) bZoomingOut = false;
}


void ANavigator::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANavigator, CurrentPosition);
	DOREPLIFETIME(ANavigator, CurrentRotation);
}