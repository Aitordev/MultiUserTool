// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
//#include "XPlayerController.h"
#include "Navigator.h"
//#include "Neuron.h"
//#include "MyPaperSpriteActor.h"
//#include "CodeGeneratedNeuron.h"
//#include "ImpostorNeuronForest.h"
#include "MultiUserToolGameMode.generated.h"



/**
 * 
 */
UCLASS()
class MULTIUSERTOOL_API AMultiUserToolGameMode : public AGameMode
{
	GENERATED_BODY()
protected:
		AMultiUserToolGameMode(const FObjectInitializer& ObjectInitializer);
	
		virtual void PreInitializeComponents() override;

		virtual void InitGameState();

		/* Called once on every new player that enters the gamemode */
		//virtual FString InitNewPlayer(class APlayerController* NewPlayerController, const TSharedPtr<const FUniqueNetId>& UniqueId, const FString& Options, const FString& Portal = TEXT(""));

		/* The teamnumber assigned to Players */
		//int32 PlayerTeamNum;
		//ANeuron *LAS_NEURONAS;
		//int NUM_NEURONAS;
		int max_neuronas;
		/* Handle for efficient management of DefaultTimer timer */
		FTimerHandle TimerHandle_DefaultTimer;
		

		//virtual void DefaultTimer();

		virtual void StartMatch();

		//virtual void OnNightEnded();

		//virtual void SpawnDefaultInventory(APawn* PlayerPawn);

		/**
		* Make sure pawn properties are back to default
		* Also a good place to modify them on spawn
		*/
		//virtual void SetPlayerDefaults(APawn* PlayerPawn) override;

	
		void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
		//FString InitNewPlayer(class APlayerController* NewPlayerController, const TSharedPtr<const FUniqueNetId>& UniqueId, const FString& Options, const FString& Portal);
		void CreateNeuronForest(int n, float d);
		void CreateRandomNeuronForest(int n, float d);
		//void CreateRandomImpostorForest(int n, float d);

		/************************************************************************/
		/* Player Spawning                                                      */
		/************************************************************************/

		/* Don't allow spectating of bots */
		//virtual bool CanSpectate_Implementation(APlayerController* Viewer, APlayerState* ViewTarget) override;

		//virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

		/* Always pick a random location */
		//virtual bool ShouldSpawnAtStartSpot(AController* Player) override;

		//virtual bool IsSpawnpointAllowed(APlayerStart* SpawnPoint, AController* Controller);

		//virtual bool IsSpawnpointPreferred(APlayerStart* SpawnPoint, AController* Controller);

		/** returns default pawn class for given controller */
		//virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

		//ANeuron *AddNeurona();
		//ANeuron *ShowNeurona();
		//ACodeGeneratedNeuron *AddCodeGeneratedNeurona();
		//void ClearCodeGeneratedNeurona();
		//bool FreeNeuronas();
		//bool FreeTheNeuronForest();
		//bool FreeTheImpostorForest();
		//void ClearShowNeurona();
		//void LoadNeuronScene(char* fich);
		//void CreateImpostorNeuronForest(int n, float d);
		//void ClearAll();
public:
	void PlayReplay();
};
