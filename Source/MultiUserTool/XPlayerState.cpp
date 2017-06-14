// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiUserTool.h"
#include "XPlayerState.h"
#include "NavigatorHUD.h"
#include "SlateChat.h"

AXPlayerState::AXPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	/* AI will remain in team 0, players are updated to team 1 through the GameMode::InitNewPlayer */
	TeamNumber = 0;
}

void AXPlayerState::Reset()
{
	Super::Reset();
}

void AXPlayerState::SetTeamNumber(int32 NewTeamNumber)
{
	TeamNumber = NewTeamNumber;
}


int32 AXPlayerState::GetTeamNumber() const
{
	return TeamNumber;
}
void AXPlayerState::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AXPlayerState, TeamNumber);
}

bool AXPlayerState::UserChatRPC_Validate(const FSChatMsg& newmessage, const FText& userDestination, const bool isPrivate)
{
	return true;
}
void AXPlayerState::UserChatRPC_Implementation(const FSChatMsg& newmessage, const FText& userDestination, const bool isPrivate)
{
	UMyAdvancedFriendsGameInstance*	GI = Cast<UMyAdvancedFriendsGameInstance>(GetGameInstance());
	if (isPrivate)
	{
		if (GI)	GI->NewMessage(newmessage, userDestination);
		UserChatP(newmessage, userDestination);
	}
	else
	{
		if (GI)	GI->NewMessage(newmessage, FText::FromString(TEXT("Public")));
		UserChat(newmessage);
	}
}
bool AXPlayerState::UserChat_Validate(const FSChatMsg& newmessage)
{
	return true;
}
void AXPlayerState::UserChat_Implementation(const FSChatMsg& newmessage)
{
	APlayerController* MyCon;
	ANavigatorHUD* MyHud;

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator) // find all controllers
	{
		MyCon = Cast<APlayerController>(*Iterator);
		if (MyCon)
		{
			MyHud = Cast<ANavigatorHUD>(MyCon->GetHUD());
			if (MyHud && MyHud->MyUIWidget.IsValid())
				MyHud->MyUIWidget->AddMessage(newmessage); // place the chat message on this player controller
		}
	}
}

bool AXPlayerState::UserChatP_Validate(const FSChatMsg& newmessage, const FText& userDestination)
{
	return true;
}
void AXPlayerState::UserChatP_Implementation(const FSChatMsg& newmessage, const FText& userDestination)
{
	APlayerController* MyCon;
	ANavigatorHUD* MyHud;

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator) // find all controllers
	{
		MyCon = Cast<APlayerController>(*Iterator);
		AXPlayerState* MyPS = Cast<AXPlayerState>(MyCon->PlayerState);
		if (MyCon && MyPS && userDestination.ToString().Compare(MyPS->PlayerName) == 0)
		{
			MyHud = Cast<ANavigatorHUD>(MyCon->GetHUD());
			if (MyHud && MyHud->MyUIWidget.IsValid())
				MyHud->MyUIWidget->AddMessage(newmessage); // place the chat message on this player controller
		}
	}
}
