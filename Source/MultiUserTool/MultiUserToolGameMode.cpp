// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiUserTool.h"
#include "MultiUserToolGameMode.h"
#include "XGameState.h"
#include "XPlayerController.h"
#include "XPlayerState.h"
#include "Public/MyDefaultPawn.h"
#include "NavigatorHUD.h"
#include "Local.h"
#include "LightControls.h"
//#include "MyHUD.h"
#include "Funciones.h"

UWorld *GWORLD=NULL;
float FOREST_NUMBER = 2.0f;//125 neuronas
float FOREST_GAP = 300.0f;
ANeuron *NeuronToShow = nullptr;

AMultiUserToolGameMode::AMultiUserToolGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	//HUDClass = AMyHUD::StaticClass(); 
	HUDClass = ANavigatorHUD::StaticClass();
	/* Assign the class types used by this gamemode */
	//DefaultPawnClass = ANavigator::StaticClass();
	DefaultPawnClass = AMyDefaultPawn::StaticClass();
	PlayerControllerClass = AXPlayerController::StaticClass();
	PlayerStateClass = AXPlayerState::StaticClass();
	GameStateClass = AXGameState::StaticClass();
	//INICIALIZAMOS LAS VARIABLES
	//PlayerTeamNum = 1;
	BOSQUE_NEURONAS=NULL;
	max_neuronas=0;
	NUM_NEURONAS = 0;
	
}

void AMultiUserToolGameMode::InitGameState()
{
	Super::InitGameState();

	AXGameState* MyGameState = Cast<AXGameState>(GameState);
	if (MyGameState)
	{
		//MyGameState->ElapsedGameMinutes = TimeOfDayStart;
	}
}


void AMultiUserToolGameMode::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	/* Set timer to run every second */
	//GetWorldTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &AMultiUserToolGameMode::DefaultTimer, GetWorldSettings()->GetEffectiveTimeDilation(), true);
	CreateNeuronForest((int)FOREST_NUMBER, FOREST_GAP);
}

void AMultiUserToolGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	UGameInstance* GameInstance = GetGameInstance(); 
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("init replay"));
	if (GameInstance) { 
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("init gi replay"));
		GameInstance->StartRecordingReplay(FString("replay"), FString("replay")); 
	}
}


void AMultiUserToolGameMode::PlayReplay()
{
	UGameInstance* GameInstance = GetGameInstance();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("GM playreplay"));
	if (GameInstance) { 
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("GM GI playreplay"));
		GameInstance->StopRecordingReplay();
		GameInstance->PlayReplay(FString("replay"));
	}
}


/*
FString AMultiUserToolGameMode::InitNewPlayer(class APlayerController* NewPlayerController, const TSharedPtr<const FUniqueNetId>& UniqueId, const FString& Options, const FString& Portal)
{
	FString Result = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
	AXPlayerState* NewPlayerState = Cast<AXPlayerState>(NewPlayerController->PlayerState);
	if (NewPlayerState)
	{
		NewPlayerState->SetTeamNumber(PlayerTeamNum);
	}

	return Result;
}
*/
void AMultiUserToolGameMode::StartMatch()
{
	Super::StartMatch();
}
/*
void AMultiUserToolGameMode::ClearAll()
{
	ClearShowNeurona();
	ClearCodeGeneratedNeurona();
	FreeNeuronas();//NEURONAS SUELTAS O GENERADAS
	FreeTheNeuronForest();//
	FreeTheImpostorForest();
	GetWorld()->ForceGarbageCollection(true);
}

void AMultiUserToolGameMode::DefaultTimer()
{
	extern int SCENE_TO_LOAD;
	extern int NEURONS_TO_SPAW;
	extern int NEURONS_TO_SHOW;
	extern int GEN_NEURONS_TO_SPAW;
	extern bool ADD_NEURON_FOREST;
	extern bool ADD_BIG_NEURON_FOREST;
	extern bool ADD_IMPOSTORES;
	extern bool CLEAN_ALL;
	extern bool FREE_NEURON_FOREST;
	extern char SCENE_FILE[256];
	extern bool NEW_MATERIAL;
	extern UMaterial *THE_NEW_MATERIAL;
	extern int MATERIAL_TYPE;

	if (SCENE_TO_LOAD) {
		SCENE_TO_LOAD = 0;
		LoadNeuronScene(SCENE_FILE);
	}
	if (NEURONS_TO_SPAW) {
		NEURONS_TO_SPAW = 0;
		AddNeurona();
	}
	if (NEURONS_TO_SHOW) {
		if (ShowNeurona() != nullptr) {
			NEURONS_TO_SHOW = 0;
		}
	}
	if (NEW_MATERIAL) {
		NEW_MATERIAL = false;
		if (NeuronToShow && THE_NEW_MATERIAL) {
			NeuronToShow->SetMaterial(MATERIAL_TYPE);// THE_NEW_MATERIAL);
		}
		/*
		if (GNEURON && THE_NEW_MATERIAL) {
			GNEURON->SetMaterial(THE_NEW_MATERIAL);
		}
		*//*
	}
	if (GEN_NEURONS_TO_SPAW) {
		GEN_NEURONS_TO_SPAW = 0;
		AddCodeGeneratedNeurona();
	}
	if (ADD_NEURON_FOREST) {
		ADD_NEURON_FOREST = false;
		CreateNeuronForest((int)FOREST_NUMBER, FOREST_GAP);
	}
	
	if (ADD_BIG_NEURON_FOREST) {
		ADD_BIG_NEURON_FOREST = false;
		CreateNeuronForest(5,400.0f);
	}

	if(ADD_IMPOSTORES) {
		ADD_IMPOSTORES = false;
		CreateRandomImpostorForest(10, 200.0f);
		//CreateImpostorNeuronForest((int)FOREST_NUMBER, FOREST_GAP);
	}
	
	if (FREE_NEURON_FOREST) {
		FREE_NEURON_FOREST = false;
		FreeTheNeuronForest();
	}
	if (CLEAN_ALL) {
		CLEAN_ALL = false;
		ClearAll();
	}
}

bool AMultiUserToolGameMode::FreeNeuronas()
{
	NEURON_REF *ref;
	if (MAX_NEURONAS) {
		int max_neuronas = MAX_NEURONAS;
		MAX_NEURONAS = 0;
		SELECTED_NEURON = 0;
		for (int i = 0; i<max_neuronas; i++) {
			ref = NEURONAS[i];
			if (ref->Neurona) ref->Neurona->SetLifeSpan(1.0f); //GetWorld()->DestroyActor(ref->Neurona, true, true);
			if (ref->NeuronaGenerada) ref->NeuronaGenerada->SetLifeSpan(1.0f); //GetWorld()->DestroyActor(ref->NeuronaGenerada, true, true);
			delete(ref);
			NEURONAS[i] = nullptr;
		}
		MAX_NEURONAS = 0;
		return true;
	}
	return false;
}

bool AMultiUserToolGameMode::FreeTheNeuronForest()
{
	//ANeuron *Bosque = BOSQUE_NEURONAS;
	if (BOSQUE_NEURONAS) {
		for (int i = 0; i<NUM_NEURONAS; i++) {
			//GetWorld()->DestroyActor(BOSQUE_NEURONAS[i], true, true);
			//GetWorld()->RemoveActor(BOSQUE_NEURONAS[i], false);
			//GetWorld()->RemoveNetworkActor(BOSQUE_NEURONAS[i]);
			BOSQUE_NEURONAS[i]->SetLifeSpan(1.0f);//LIMITAMOS SU VIDA
		}
		//free(BOSQUE_NEURONAS);
		BOSQUE_NEURONAS = NULL;
		NUM_NEURONAS = 0;
		return true;
	}
	return false;
}

bool AMultiUserToolGameMode::FreeTheImpostorForest()
{
	//ANeuron *Bosque = BOSQUE_NEURONAS;
	if (BOSQUE_SPRITE_NEURONAS) {
		int n = NUM_SPRITE_NEURONAS;
		NUM_SPRITE_NEURONAS = 0;
		for (int i = 0; i<n; i++) {
			//GetWorld()->DestroyActor(BOSQUE_NEURONAS[i], true, true);
			//GetWorld()->RemoveActor(BOSQUE_NEURONAS[i], false);
			//GetWorld()->RemoveNetworkActor(BOSQUE_NEURONAS[i]);
			BOSQUE_SPRITE_NEURONAS[i]->SetLifeSpan(1.0f);//LIMITAMOS SU VIDA
		}
		//free(BOSQUE_NEURONAS);
		BOSQUE_SPRITE_NEURONAS = NULL;
		NUM_SPRITE_NEURONAS = 0;
		return true;
	}
	return false;
}

void AMultiUserToolGameMode::LoadNeuronScene(char* fich) {
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
*/

void AMultiUserToolGameMode::CreateRandomNeuronForest(int n, float d)
{
	extern int NNEUR;
	FVector Pos;
	FRotator Rot;
	float R = d;
	int nx = n;
	int ny = n;
	int nz = n;
	
	max_neuronas = (2 * nx + 1)*(2 * ny + 1)*(2 * nz + 1);
	BOSQUE_NEURONAS = (ANeuron **)malloc(max_neuronas*sizeof(ANeuron *));
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
	NNEUR = NUM_NEURONAS;
}
/*
void AMultiUserToolGameMode::CreateRandomImpostorForest(int n, float d)
{
	//extern int NNEUR;
	FVector Pos;
	FRotator Rot;
	float R = d;// 300.0f;
	int nx = n;
	int ny = n;
	int nz = n;
	int max_neuronas = (2 * nx + 1)*(2 * ny + 1)*(2 * nz + 1);
	FreeTheImpostorForest();
	BOSQUE_SPRITE_NEURONAS = (AMyPaperSpriteActor **)malloc(max_neuronas * sizeof(AMyPaperSpriteActor *));
	if (GetWorld()) {
		for (int k = -nz; k <= nz; k++) {
			for (int i = -nx; i <= nx; i++) {
				for (int j = -ny; j <= ny; j++) {
					Pos = FVector(R * i, R * j, R * k) + RandomVector(0.5f*R);
					Rot = RandomRotator(0.0f);
					BOSQUE_SPRITE_NEURONAS[NUM_SPRITE_NEURONAS++] = GetWorld()->SpawnActor<AMyPaperSpriteActor>(AMyPaperSpriteActor::StaticClass(), Pos, Rot);
				}
			}
		}
	}
	//NNEUR = NUM_NEURONAS;
}


*/
void AMultiUserToolGameMode::CreateNeuronForest(int n, float d)
{
	GWORLD = GetWorld();
	CreateRandomNeuronForest(n, d);
}
/*
void AMultiUserToolGameMode::CreateImpostorNeuronForest(int n, float d)
{
	GWORLD = GetWorld();
	AImpostorNeuronForest *ImpostorForest = GetWorld()->SpawnActor<AImpostorNeuronForest>(AImpostorNeuronForest::StaticClass(),FVector(0,0,0),FRotator(0,0,0));
	//CreateImpostorRandomNeuronForest(n, d);
}
*/
/*
void AMultiUserToolGameMode::SetPlayerDefaults(APawn* PlayerPawn)
{
	Super::SetPlayerDefaults(PlayerPawn);
}
*/
/* Used by RestartPlayer() to determine the pawn to create and possess when a bot or player spawns 
UClass* AMultiUserToolGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}
*//*
bool AMultiUserToolGameMode::ShouldSpawnAtStartSpot(AController* Player)
{
	// Always pick a random location 
	return false;
}


bool AMultiUserToolGameMode::CanSpectate_Implementation(APlayerController* Viewer, APlayerState* ViewTarget)
{
	// Don't allow spectating of other non-player bots 
	return false;// (ViewTarget && !ViewTarget->bIsABot);
}

AActor* AMultiUserToolGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<APlayerStart*> PreferredSpawns;
	TArray<APlayerStart*> FallbackSpawns;

	// Get all playerstart objects in level 
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);

	// Split the player starts into two arrays for preferred and fallback spawns 
	for (int32 i = 0; i < PlayerStarts.Num(); i++)
	{
		APlayerStart* TestStart = Cast<APlayerStart>(PlayerStarts[i]);

		if (TestStart && IsSpawnpointAllowed(TestStart, Player))
		{
			if (IsSpawnpointPreferred(TestStart, Player))
			{
				PreferredSpawns.Add(TestStart);
			}
			else
			{
				FallbackSpawns.Add(TestStart);
			}
		}

	}

	// Pick a random spawnpoint from the filtered spawn points 
	APlayerStart* BestStart = nullptr;
	if (PreferredSpawns.Num() > 0)
	{
		BestStart = PreferredSpawns[FMath::RandHelper(PreferredSpawns.Num())];
	}
	else if (FallbackSpawns.Num() > 0)
	{
		BestStart = FallbackSpawns[FMath::RandHelper(FallbackSpawns.Num())];
	}

	// If we failed to find any (so BestStart is nullptr) fall back to the base code 
	return BestStart ? BestStart : Super::ChoosePlayerStart_Implementation(Player);
}

ANeuron *AMultiUserToolGameMode::AddNeurona()
{
	float R = 500.0f;
	FVector Pos = FVector(0, 0, 100)+RandomVector(R);
	FRotator Rot = RandomRotator(90.0f);
	if (MAX_NEURONAS == 0) {
		Pos = FVector(0, 0, 100);
		Rot = FRotator(0, 0, 0);
	}
	ANeuron *neurona = GetWorld()->SpawnActor<ANeuron>(ANeuron::StaticClass(), Pos, Rot);
	NEURONS_POS[MAX_NEURONAS] = Pos;
	NEURONAS[MAX_NEURONAS++] = new NEURON_REF(neurona, Pos, Rot);
	return neurona;
}



void AMultiUserToolGameMode::ClearShowNeurona()
{
	if (NeuronToShow) {
		NeuronToShow->K2_DestroyActor();
		NeuronToShow = nullptr;
	}
}
ANeuron *AMultiUserToolGameMode::ShowNeurona()
{
	//static ANeuron *neurona = nullptr;
	//static ANeuron *destroyed_neuron = nullptr;
	float R = 500.0f;
	FVector Pos = FVector(0, 0, 100);
	FRotator Rot = RandomRotator(0.0f);
	if (NeuronToShow) {
		//destroyed_neuron = neurona;
		//GetWorld()->DestroyActor(neurona, true, true);
		//Sleep(1000);
		NeuronToShow->K2_DestroyActor();
		NeuronToShow = nullptr;
		//return nullptr;
	}
	NeuronToShow = GetWorld()->SpawnActor<ANeuron>(ANeuron::StaticClass(), Pos, Rot);
	return NeuronToShow;
}


ACodeGeneratedNeuron *GNEURON = nullptr;

void AMultiUserToolGameMode::ClearCodeGeneratedNeurona()
{
	if (GNEURON) {
		GetWorld()->DestroyActor(GNEURON, true, false);
		GNEURON = nullptr;
	}
}

ACodeGeneratedNeuron *AMultiUserToolGameMode::AddCodeGeneratedNeurona()
{
	static int n = 0;
	float R = 1500.0f;
	FVector Pos = FVector(0, 0, 100);
	FRotator Rot = FRotator(0, 0, 0);
	if (GNEURON) {
		GetWorld()->DestroyActor(GNEURON, true, false);
		GNEURON = nullptr;
	}
	GNEURON = GetWorld()->SpawnActor<ACodeGeneratedNeuron>(ACodeGeneratedNeuron::StaticClass(), Pos, Rot);
	return GNEURON;
}


bool AMultiUserToolGameMode::IsSpawnpointAllowed(APlayerStart* SpawnPoint, AController* Controller)
{
	if (Controller == nullptr || Controller->PlayerState == nullptr)
		return true;
	return true;
}


bool AMultiUserToolGameMode::IsSpawnpointPreferred(APlayerStart* SpawnPoint, AController* Controller)
{
	if (SpawnPoint)
	{
		// Iterate all pawns to check for collision overlaps with the spawn point 
		const FVector SpawnLocation = SpawnPoint->GetActorLocation();
		for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; It++)
		{
			ACharacter* OtherPawn = Cast<ACharacter>(*It);
			if (OtherPawn)
			{
				const float CombinedHeight = (SpawnPoint->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + OtherPawn->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()) * 2.0f;
				const float CombinedWidth = SpawnPoint->GetCapsuleComponent()->GetScaledCapsuleRadius() + OtherPawn->GetCapsuleComponent()->GetScaledCapsuleRadius();
				const FVector OtherLocation = OtherPawn->GetActorLocation();

				// Check if player overlaps the playerstart
				if (FMath::Abs(SpawnLocation.Z - OtherLocation.Z) < CombinedHeight && (SpawnLocation - OtherLocation).Size2D() < CombinedWidth)
				{
					return false;
				}
			}
		}
	}

	return false;
}
*/



