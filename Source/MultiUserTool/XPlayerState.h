// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "NavigatorHUD.h"
#include "XPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MULTIUSERTOOL_API AXPlayerState : public APlayerState
{
	GENERATED_BODY()


public:
	
	AXPlayerState(const FObjectInitializer& ObjectInitializer);
	/* Team number assigned to player */
	UPROPERTY(Transient, Replicated)
		int32 TeamNumber;

	virtual void Reset() override;
	void SetTeamNumber(int32 NewTeamNumber);

	UFUNCTION(BlueprintCallable, Category = "Teams")
		int32 GetTeamNumber() const;

	/** for player to player rpc you need to first call the message on the server*/
	// first rpc for the server
	UFUNCTION(Server, Reliable, WithValidation) 
	virtual void UserChatRPC(const FSChatMsg& newmessage, const FText& userDestination, const bool isPrivate);
	
	// second rpc for all the clients
	UFUNCTION(NetMulticast, Reliable, WithValidation) 
	// then the server calls the function with a multicast that executes on all clients and the server
	virtual void UserChat(const FSChatMsg& newmessage);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	// then the server calls the function with a multicast that executes on all clients and the server
	virtual void UserChatP(const FSChatMsg& newmessage, const FText& userDestination);
};
