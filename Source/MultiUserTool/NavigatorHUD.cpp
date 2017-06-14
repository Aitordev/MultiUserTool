// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiUserTool.h"
#include "NavigatorHUD.h"
#include "SlateChat.h"
#include "XPlayerState.h"
#include "XPlayerController.h"
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>


ANavigatorHUD::ANavigatorHUD()
{

}

void ANavigatorHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MyPC = GetOwningPlayerController();
	MyDP = Cast<AMyDefaultPawn>(MyPC->GetPawn());

	//Set player name
	GI = Cast<UMyAdvancedFriendsGameInstance>(GetGameInstance());
	if (GEngine && GEngine->GameViewport) // make sure our screen is ready for the widget
	{
		SAssignNew(MyUIWidget, SSlateChat).OwnerHUD(this); // add the widget and assign it to the var
		GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(MyUIWidget.ToSharedRef()));
	}

}

void ANavigatorHUD::DrawHUD()
{
	Super::DrawHUD();

	if (!MyPC)
	{
		MyPC = GetOwningPlayerController();
		//AddMessageBP(2, TEXT(""), TEXT("Welcome. Press Enter to chat."), false); // random Welcome message shown to the local player. To be deleted. note type 2 is system message and username is blank
		MyDP = Cast<AMyDefaultPawn>(MyPC->GetPawn());
		return;
	}

	if (MyPC->WasInputKeyJustPressed(EKeys::Enter)) {
		if (!isGuest && MyUIWidget.IsValid() && MyUIWidget->ChatInput.IsValid()){
			FSlateApplication::Get().SetKeyboardFocus(MyUIWidget->ChatInput); // When the user presses Enter he will focus his keypresses on the chat input bar
		}
	}
	
	if (MyPC->WasInputKeyJustPressed(EKeys::Y)) {
		AXPlayerController* MyPCC = Cast<AXPlayerController>(MyPC);
		MyPCC->PlayReplay();
	}
	if (MyPC->WasInputKeyJustPressed(EKeys::Tab)) {
		//AXPlayerState* MyPS = Cast<AXPlayerState>(MyPC->PlayerState);
		int32 i = 0;
		bool recorrido = true;
		CurrentPawnCameraName = MyDP->CurrentFloatingName;

		for (TObjectIterator<AMyDefaultPawn> Itr; Itr; ++Itr)
		{
			i++;
			CurrentPawn = Cast<AMyDefaultPawn>(*Itr);
			if (i > CurrentPawnCameraNumber && !CurrentPawnCameraName.Equals(CurrentPawn->CurrentFloatingName) && !CurrentPawn->CurrentFloatingName.Equals(TEXT("guest"))) {
				UE_LOG(LogTemp, Warning, TEXT("camera %s"), *CurrentPawn->CurrentFloatingName);
				GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::White, FString::Printf(TEXT("%s Camera"), *CurrentPawn->CurrentFloatingName), false, FVector2D(2, 2));
				CurrentPawnCameraNumber = i;
				MyPC->SetViewTargetWithBlend(CurrentPawn);
				recorrido = false;
				isCurrentPawn = true;
				break;
			}
		}
		if (recorrido)
		{
			UE_LOG(LogTemp, Warning, TEXT("Dentro recorrido"));
			GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::White, TEXT("Main Camera"),false, FVector2D(2, 2));
			CurrentPawnCameraNumber = 0;
			MyPC->SetViewTargetWithBlend(MyDP);
			isCurrentPawn = false;
		}
	}
}

void ANavigatorHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (GI && MyPC)
	{
		AXPlayerState* MyPS = Cast<AXPlayerState>(MyPC->PlayerState);
		MyDP = Cast<AMyDefaultPawn>(MyPC->GetPawn());
		if (MyPS)
		{
			MyPS->SetPlayerName(GI->userName);
		}
		if (MyDP)
		{
			MyDP->SetCurrentFloatingName(GI->userName);
		}
		
		HideinputChat(GI->IsGuest);

	}
	if (isCurrentPawn && MyPC && MyDP && CurrentPawn)
	{
		//MyPC->SetControlRotation(CurrentPawn->GetActorRotation());
		MyPC->SetControlRotation(CurrentPawn->GetActorRotation());
		//MyDP->SetActorRotation(CurrentPawn->GetActorRotation());
		//MyDP->SetActorLocation(CurrentPawn->GetActorLocation());
	}
}


void ANavigatorHUD::AddMessageBP(const int32 Type, const FString& Username, const FString& Text, const bool Replicate)
{
	if (!MyPC || !MyUIWidget.IsValid())
		return;

	FSChatMsg newmessage;
	newmessage.Init(Type, FText::FromString(Username), FText::FromString(Text)); // initialize our struct and prep the message
	if (newmessage.Type > 0)
		if (Replicate)
		{
			AXPlayerState* MyPS = Cast<AXPlayerState>(MyPC->PlayerState);
			if (MyPS)
				MyPS->UserChatRPC(newmessage, FText::FromString(TEXT("")), false); // Send the complete chat message to the PlayerState so it can be replicated then displayed
		}
		else
			MyUIWidget->AddMessage(newmessage); // Send a local message to this client only, no one else receives it
}

void ANavigatorHUD::HideinputChat(const bool Hide)
{
	if (Hide && MyUIWidget.IsValid()) 
	{
		MyUIWidget->SetGuest();
	}
}







