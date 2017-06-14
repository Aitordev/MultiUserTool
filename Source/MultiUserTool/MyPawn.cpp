// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiUserTool.h"
#include "Runtime/Core/Public/HAL/FileManagerGeneric.h"
#include "Funciones.h"
#include "Neuron.h"
//#include "CodeGeneratedNeuron.h"
#include "MyHUD.h"
//#include "MyPlayerController.h"
#include "LightControls.h"
#include "Local.h"
#include "MyPawn.h"

/*
// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}
*/

//CODE
extern int SELECTED_CONTROL;
extern bool TRACKING_SLIDER2 = false;
class FFileManagerGeneric;

int AMyPawn::LoadNeuronas(char *dir)
{
	struct FConstructorStatics {
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> NavigatorMesh;
		FConstructorStatics()
			//: NavigatorMesh(TEXT("/Game/Mesh/eyeball.eyeball"))
			//: NavigatorMesh(TEXT("/Game/Mesh/Neuron/Inter_000_L0.Inter_000_L0"))
			: NavigatorMesh(TEXT("/Game/Mesh/Neuron/neuron001.neuron001"))
		{
		}
	};


	static FConstructorStatics ConstructorStatics;
	// Create static mesh component

	char fich[256];
	//TCHAR Fich[256];
	TArray< FString > FoundFiles;
	/*
	TCHAR Directory[256];
	TCHAR FileExtension[256];
	FFileManagerGeneric *Fm = new FFileManagerGeneric();

	Fm->FindFiles(FoundFiles, TEXT("/Game/Mesh/Neuron"), TEXT("uaset"));
	*/


	char name[256];
	int j = 1;
	for (int i = 0; i < 5; i++) {
		sprintf(fich, "%s/neurona%3.3d.neurona%3.3d", dir, j, j);
		sprintf(name, "neurona%3.3d", j);
		//ANSI_TO_TCHAR
		FString Fich = FString(fich);
		FString Name = FString(name);
		//TCHAR Tfich[256];
		//TCHAR Tname[256];
		Neuronas[j++] = LoadObject<UStaticMesh>(nullptr, *Name, *Fich, LOAD_None, nullptr);
		//Neuronas[j++] = LoadObject<UStaticMeshComponent>(nullptr,Tname,Tfich, LOAD_None, nullptr); //CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NavigatorMesh0"));
	}

	return j;
}


AMyPawn::AMyPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	NavigatorMesh = nullptr;
	MaxNeuronas = LoadNeuronas("/Game/Mesh/Neuron");
	if (1) {
		struct FConstructorStatics {
			ConstructorHelpers::FObjectFinderOptional<UStaticMesh> NavigatorMesh;
			FConstructorStatics()
				: NavigatorMesh(TEXT("/Game/Mesh/eyeball.eyeball"))
				//: NavigatorMesh(TEXT("/Game/Mesh/Neuron/Inter_000_L0.Inter_000_L0"))
				//: NavigatorMesh(TEXT("/Game/Mesh/Neuron/neuron001.neuron001"))
			{
			}
		};
		static FConstructorStatics ConstructorStatics;
		// Create static mesh component

		NavigatorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NavigatorMesh0"));
		NavigatorMesh->SetStaticMesh(ConstructorStatics.NavigatorMesh.Get());
	}
	else {
		NavigatorMesh = nullptr;
	}

	SetActorLocation(FVector(0.0f, 0.0f, 0.0f));
	//bAddDefaultMovementBinding = false;

	//Create our components
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	if (NavigatorMesh) {
		NavigatorMesh->AttachTo(RootComponent);
		NavigatorMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		NavigatorMesh->SetWorldScale3D(FVector(0.05f));
	}

	OurCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	OurCameraSpringArm->AttachTo(RootComponent);
	OurCameraSpringArm->SetRelativeLocationAndRotation(FVector(1500.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 180.0f));
	//OurCameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 250.0f), FRotator(-60.0f, 0.0f, 0.0f));
	OurCameraSpringArm->TargetArmLength = 1500.0f;
	OurCameraSpringArm->bEnableCameraLag = false;// true;
	OurCameraSpringArm->CameraLagSpeed = 3.0f;

	OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	OurCamera->AttachTo(OurCameraSpringArm, USpringArmComponent::SocketName);
	OurCamera->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 180.0f, 0.0f));
	OurCamera->ProjectionMode = ECameraProjectionMode::Perspective;
	OurCamera->FieldOfView = GFOV;

	OurLight2 = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLightComp"));
	OurLight2->AttachTo(OurCameraSpringArm, TEXT("LightSocket"), EAttachLocation::SnapToTarget);
	OurLight2->AddLocalRotation(FRotator(0, 0, 0));
	OurLight2->AddLocalOffset(FVector(0.0f, 0.0f, 0.0f));

	OurLight2->SetOuterConeAngle(120.0f);
	OurLight2->SetLightColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	OurLight2->SetAttenuationRadius(10000.0f);
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld() != NULL) {
		APlayerController* MyController = GetWorld()->GetFirstPlayerController();
		MyController->bShowMouseCursor = true;
		MyController->bEnableClickEvents = true;
		MyController->bEnableMouseOverEvents = true;
		EnableInput(MyController);
	}
}



void AMyPawn::OnLBD()
{
	//GLBD = true;
}
void AMyPawn::OnLBU()
{
	//GLBD = false;
}

void AMyPawn::OnKBD()
{
	//GLBD = false;
}

void AMyPawn::OnKBU()
{
	//GLBD = false;
}

void AMyPawn::OnMouseWheel(float AxisValue)
{
	GZOOM += 0.05f*AxisValue;
	if (GZOOM < 0.0f) GZOOM = 0.0f;
	if (GZOOM > 10.0f) GZOOM = 10.0f;
	GMWHEEL = AxisValue;
}

void AMyPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAxis("WheelZoom", this, &AMyPawn::OnMouseWheel);
	//YA NO GESTIONAMOS AQUI
	//InputComponent->BindAction("LBD", IE_Pressed, this, &AMyPawn::OnLBD);
	//InputComponent->BindAction("LBD", IE_Released, this, &AMyPawn::OnLBU);
	/*
	//Hook up events for "ZoomIn"
	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &AMyPawn::ZoomIn);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &AMyPawn::ZoomOut);
	InputComponent->BindAction("EndZoomIn", IE_Released, this, &AMyPawn::EndZoomIn);
	InputComponent->BindAction("EndZoomOut", IE_Released, this, &AMyPawn::EndZoomOut);

	//Hook up every-frame handling for our four axes
	InputComponent->BindAxis("MoveForward", this, &AMyPawn::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMyPawn::MoveRight);
	InputComponent->BindAxis("MoveTop", this, &AMyPawn::MoveUpDown);
	InputComponent->BindAxis("CameraPitch", this, &AMyPawn::AddControllerPitchInput);// PitchCamera);
	InputComponent->BindAxis("CameraYaw", this, &AMyPawn::AddControllerYawInput);// YawCamera);
	InputComponent->BindAxis("WheelZoom", this, &AMyPawn::WheelZoom);
	*/
}

char *GetNumberKey(int i)
{
	static char aux[256];
	int d = i - '0';
	switch (d) {
	case 0: sprintf(aux, "zero"); return aux;
	case 1: sprintf(aux, "one"); return aux;
	case 2: sprintf(aux, "two"); return aux;
	case 3: sprintf(aux, "three"); return aux;
	case 4: sprintf(aux, "four"); return aux;
	case 5: sprintf(aux, "five"); return aux;
	case 6: sprintf(aux, "six"); return aux;
	case 7: sprintf(aux, "seven"); return aux;
	case 8: sprintf(aux, "eight"); return aux;
	case 9: sprintf(aux, "nine"); return aux;
	}
	sprintf(aux, "one"); return aux;//POR DEFECTO
}

bool AMyPawn::UpdateInput(float DeltaTime)
{
	static FKey *FKEYS[256];
	static bool Inic = true;
	if (Inic) {
		int j = 0;
		for (int i = 0; i < 256; i++) FKEYS[i] = nullptr;
		FKEYS[j++] = new FKey(TEXT("LeftMouseButton"));
		FKEYS[j++] = new FKey(TEXT("RightMouseButton"));
		FKEYS[j++] = new FKey(TEXT("MiddleMouseButton"));
		//FKEYS[j++] = new FKey(TEXT("MouseWheelAxis"));
		char aux[32];
		for (int i = (int)'A'; i <= (int)'Z'; i++) {
			aux[0] = (char)i; aux[1] = 0;
			FKEYS[i] = new FKey(aux);
		}
		for (int i = (int)'0'; i <= (int)'9'; i++) {
			aux[0] = (char)i; aux[1] = 0;
			FKEYS[i] = new FKey(GetNumberKey(i));
		}
		Inic = false;
	}


	if (GetWorld() != NULL) {
		APlayerController* MyController = GetWorld()->GetFirstPlayerController();
		MyController->GetMousePosition(GFMX, GFMY);
		
		//GMWHEEL=MyController->GetInputAnalogKeyState(*FKEYS[4]);
		GLBD = MyController->IsInputKeyDown(*FKEYS[0]);
		GRBD = MyController->IsInputKeyDown(*FKEYS[1]);
		GMBD = MyController->IsInputKeyDown(*FKEYS[2]);
		for (int i = (int)'A'; i <= (int)'Z'; i++) {
			TECLAS[i] = MyController->IsInputKeyDown(*FKEYS[i]);
		}
		for (int i = (int)'0'; i <= (int)'9'; i++) {
			TECLAS[i] = MyController->IsInputKeyDown(*FKEYS[i]);
		}
		
	}
	return true;
}

bool AMyPawn::EndInput()
{
	UGLBD = GLBD;
	UGRBD = GRBD;
	UGMBD = GMBD;
	for (int i = (int)'A'; i <= (int)'Z'; i++) UTECLAS[i] = TECLAS[i];
	for (int i = (int)'0'; i <= (int)'9'; i++) UTECLAS[i] = TECLAS[i];
	return true;
}

bool AMyPawn::Pulsa(char c)
{
	return TECLAS[(int)c];
}

bool AMyPawn::KBD(char c)
{
	if (TECLAS[(int)c] && !UTECLAS[(int)c]) return true;
	return false;
}

bool AMyPawn::LBD(void)
{
	if (GLBD && !UGLBD) return true;
	return false;
}

bool AMyPawn::KBU(char c)
{
	if (UTECLAS[(int)c] && !TECLAS[(int)c]) return true;
	return false;
}

float AMyPawn::UpdateFps(float DeltaSeconds)
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


bool AMyPawn::UpdateInterface()
{
	//extern int SELECTED_CONTROL;
	if (SELECTED_CONTROL) {
		if (IS_CONTROL_CLICKED("ESCENA", LBD())) {
			CreateRandomNeuronForest(5, 500.0f);
			return true;
		}
		if (IS_CONTROL_CLICKED("GENERA", LBD())) {
			//AddCodeGeneratedNeurona();
			return true;
		}
		if (IS_CONTROL_CLICKED("ADDNEU", LBD())) {
			AddNeurona();
			return true;
		}
		if (IS_CONTROL_CLICKED("LOAD", LBD())) {
			LoadNeuronForest("//Game//scenes//LowRes_4.txt");
			return true;
		}
		char file[256];
		if (GET_CONTROL_CLICKED(file, LBD())) {
			LoadNeuronForest(file);
			return true;
		}


		return true;
	}

	return false;
}
// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateFps(DeltaTime);
	UpdateInput(DeltaTime);
	if (!UpdateInterface()) {

		if (Pulsa('Z')) GZOOM += 0.1f*DeltaTime;
		if (Pulsa('X')) GZOOM -= 0.1f*DeltaTime;
		if (GZOOM < 0.1f) GZOOM = 0.1f;
		if (GZOOM > 10.0f) GZOOM = 10.0f;
		GFOV = 10.0f / GZOOM;// OurCamera->FieldOfView = FMath::Lerp<float>(120.0f, 5.0f, GZOOM);
		OurCamera->FieldOfView = CameraFOV = GFOV;
		//OurCameraSpringArm->TargetArmLength = FMath::Lerp<float>(400.0f, 300.0f, GZOOM);
		for (int i = 0; i < MaxNeuronas; i++) {
			if (Pulsa('0' + i) && Neuronas[i]) {
				NavigatorMesh->SetStaticMesh(Neuronas[i]);
			}
		}

		if (KBD('D')) {
			FreeNeuronas();
		}
		if (KBD('N')) {
			AddNeurona();
		}
		if (KBD('B')) {
			CreateRandomNeuronForest(5, 500.0f);
		}

		if (KBD('G')) {
			//AddCodeGeneratedNeurona();
		}
		if (KBD('W')) {
			SHOW_WIRES = !SHOW_WIRES;
		}
		if (KBD('I')) {
			SHOW_HUD_INFO = !SHOW_HUD_INFO;
		}
		if (KBD('C')) {
			SHOW_HUD_INTERFACE = !SHOW_HUD_INTERFACE;
		}

		if (GMBD && !TRACKING_SLIDER2) {
			CameraAzm = AZM = 180.0f*(GFMX - OX) / OX;
			CameraElv = ELV = 90.0f*(GFMY - OY) / OY;
			CameraFOV = GFOV;
		}
	}

	UpdateCamera();
	SELECTED_NEURON = SelectNeuron();
	EndInput();
}

//#define FIXED_AIM   0
//#define FLIGHT		1

float LIGHT_AZM=0.0f;
float LIGHT_ELV = 0.0f;
float LIGHT_INT = 1.0f;
float LIGHT_ANG = 90.0f;
float LIGHT_ATN = 10000.0f;
float LIGHT_RED = 1.0f;
float LIGHT_GREEN = 1.0f;
float LIGHT_BLUE = 1.0f;

void AMyPawn::UpdateCamera()
{
	if (SELECTED_CONTROL || TRACKING_SLIDER2) return;
	CameraUpdateMode = 0;
	CameraUp = K;
	if (MAX_NEURONAS) {
		//CameraAim = NEURONS_POS[MAX_NEURONAS - 1];// FVector(0.0f, 0.0f, 0.0f);
		CameraAim = FVector(0.0f, 0.0f, 0.0f);
	}
	else {
		CameraAim = FVector(0.0f, 0.0f, 0.0f);
	}
	//CameraAzm = 45.0f;
	//CameraElv = 45.0f;
	CameraDis = DIS = 1500.0f;
	CameraPos = CameraAim + versor(CameraAzm, CameraElv)*CameraDis;
	Cx = versor(CameraAim - CameraPos);
	Cy = -versor(cross(CameraUp, Cx));
	Cz = cross(Cx, Cy);

	switch (CameraUpdateMode) {
	case FIXED_AIM://AIMED
		CameraPos = CameraAim + versor(CameraAzm, CameraElv)*CameraDis;
		//PODRIAMOS AJUSTAR EL FOV


		break;
	case FLIGHT://NAVEGACION

		break;
	}
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

	OurLight2->AddLocalRotation(FRotator(0, 0, 0));
	OurLight2->AddLocalOffset(FVector(LIGHT_ELV,180+LIGHT_AZM, 0.0f));

	OurLight2->SetOuterConeAngle(120.0f);
	OurLight2->SetLightColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	OurLight2->SetAttenuationRadius(10000.0f);
}


ANeuron *AMyPawn::AddNeurona()
{
	float R = 500.0f;
	FVector Pos = RandomVector(R);
	FRotator Rot = RandomRotator(90.0f);
	ANeuron *neurona = GetWorld()->SpawnActor<ANeuron>(ANeuron::StaticClass(), Pos, Rot);
	NEURONAS[MAX_NEURONAS++] = new NEURON_REF(neurona, Pos, Rot);
	return neurona;
}

/*

ACodeGeneratedNeuron *AMyPawn::AddCodeGeneratedNeurona()
{
	static int n = 0;
	float R = 500.0f;
	FVector Pos = RandomVector(R);
	FRotator Rot = RandomRotator(90.0f);
	if (n == 0) Pos = FVector(0, 0, 0);
	n++;
	//NEURONAS[NUM_NEURONAS++] 
	ACodeGeneratedNeuron *neurona = GetWorld()->SpawnActor<ACodeGeneratedNeuron>(ACodeGeneratedNeuron::StaticClass(), Pos, Rot);
	NEURONAS[MAX_NEURONAS++] = new NEURON_REF(neurona, Pos, Rot);
	return neurona;
}
*/

bool AMyPawn::FreeNeuronas()
{
	NEURON_REF *ref;
	if (MAX_NEURONAS) {
		int max_neuronas = MAX_NEURONAS;
		MAX_NEURONAS = 0;
		for (int i = 0; i<max_neuronas; i++) {
			ref = NEURONAS[i];
			if (ref->Neurona) GetWorld()->DestroyActor(ref->Neurona, true, true);
			//if (ref->NeuronaGenerada) GetWorld()->DestroyActor(ref->NeuronaGenerada, true, true);
			delete(ref);
			NEURONAS[i] = nullptr;
		}
		MAX_NEURONAS = 0;
		return true;
	}
	return false;
}



FVector AMyPawn::Project(FVector &Xa)
{
	FVector DX = Xa - CameraPos;
	float X = dot(DX, Cy);
	float Y = -dot(DX, Cz);
	float Z = dot(DX, Cx);
	float f = OX / TAN(CameraFOV / 2.0f);
	return  Xa;// FVector(OX + f*X / Z, OY + f*Y / Z, Z); //FVector(X, Y, Z); //FVector(OX + f*X / Z, OY + f*Y / Z, Z); //FVector(X,Y,Z); //DX;// FVector(OX + f*X / Z, OY + f*Y / Z, Z);
}





int AMyPawn::SelectNeuron()
{
	FVector Xp;
	FVector MousePos = FVector(GFMX, GFMY, 0.0f);
	int isel = -1;
	float d, dmin = 100000.0f;
	d = 0.0f;
	//CameraPos = CameraAim + versor(CameraAzm, CameraElv)*CameraDis;
	for (int i = 0; i < MAX_NEURONAS; i++) {
		NEURONS_POS[i] = NEURONAS[i]->Pos;
		Xp = Project(NEURONAS[i]->Pos);
		d = DXY(Xp, MousePos);
		if (d < dmin) {
			dmin = d;
			isel = i;
		}
		NEURONS_POS[i] = Xp;// CameraPos;// CameraPos;// NEURONAS[i]->Pos;// CameraPos;// Xp;
	}
	return isel;
}



void AMyPawn::CreateRandomNeuronForest(int n, float d)
{
	//extern int NNEUR;
	FVector Pos;
	FRotator Rot;
	float R = d;// 300.0f;
	int nx = n;
	int ny = n;
	int nz = n;
	int max_neuronas = (2 * nx + 1)*(2 * ny + 1)*(2 * nz + 1);
	FreeTheNeuronForest();
	BOSQUE_NEURONAS = (ANeuron **)malloc(max_neuronas * sizeof(ANeuron *));
	NUM_NEURONAS = 0;
	if (GetWorld()) {
		for (int k = -nz; k <= nz; k++) {
			for (int i = -nx; i <= nx; i++) {
				for (int j = -ny; j <= ny; j++) {
					Pos = FVector(R * i, R * j, R * k) + RandomVector(0.5f*R);
					Rot = RandomRotator(90.0f);
					BOSQUE_NEURONAS[NUM_NEURONAS++] = GetWorld()->SpawnActor<ANeuron>(ANeuron::StaticClass(), Pos, Rot);
				}
			}
		}
	}
	//NNEUR = NUM_NEURONAS;
}

bool AMyPawn::FreeTheNeuronForest()
{
	if (BOSQUE_NEURONAS) {
		for (int i = 0; i<NUM_NEURONAS; i++) {
			GetWorld()->DestroyActor(BOSQUE_NEURONAS[i], true, true);
		}
		free(BOSQUE_NEURONAS);
		BOSQUE_NEURONAS = NULL;
		NUM_NEURONAS = 0;
		return true;
	}
	return false;
}

void AMyPawn::LoadNeuronForest(char* fich) {
	FVector Pos;
	FRotator Rot;
	float X, Y, Z;
	int RotY;
	int S;
	extern int NNEUR;
	FreeTheNeuronForest();
	FILE *f = fopen(fich, "r");
	if (f) {
		float version;
		int n;
		char NombreNeurona[32];
		fscanf(f, "%f", &version);
		fscanf(f, "%d", &n);
		int max_neuronas = n;
		BOSQUE_NEURONAS = (ANeuron **)malloc(n * sizeof(ANeuron *));
		while (fscanf(f, "%f %f %f %d %d %s", &X, &Y, &Z, &RotY, &S, NombreNeurona) != EOF) {
			//AÑADIR LA NEURONA
			Pos = 10.0f*FVector(X, Y, Z);
			Rot = FRotator(0.0f, (float)RotY, 0.0f);
			BOSQUE_NEURONAS[NUM_NEURONAS++] = GetWorld()->SpawnActor<ANeuron>(ANeuron::StaticClass(), Pos, Rot);
		}
		fclose(f);
	}
}





