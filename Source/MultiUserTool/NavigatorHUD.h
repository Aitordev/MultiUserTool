// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "Public/MyAdvancedFriendsGameInstance.h"
#include "Public/MyDefaultPawn.h"
#include "NavigatorHUD.generated.h"

class SSlateChat;

/**
 * 
 */
UCLASS()
class MULTIUSERTOOL_API ANavigatorHUD : public AHUD
{
	GENERATED_BODY()
public:
		ANavigatorHUD();
		TSharedPtr<SSlateChat> MyUIWidget; // Reference to the main chat widget
		APlayerController* MyPC;
		AMyDefaultPawn* MyDP;

		FString CurrentPawnCameraName = "";
		FString MainPawnCameraName = "";
		int32 CurrentPawnCameraNumber = 0;
		AMyDefaultPawn* CurrentPawn;
		bool isCurrentPawn = false;
		bool isGuest = false;

		UMyAdvancedFriendsGameInstance* GI;
		UFUNCTION(BlueprintCallable, Category = "Chat")
		void AddMessageBP(const int32 Type, const FString& Username, const FString& Text, const bool Replicate); // A Blueprint function you can use to place messages in the chat box during runtime
		void HideinputChat(const bool Hide); // hide input if guest

protected:
	virtual void PostInitializeComponents() override;
		virtual void DrawHUD() override;
		virtual void Tick(float DeltaSeconds) override;
};
