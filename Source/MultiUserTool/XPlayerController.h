// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Json.h"
#include "Public/MyAdvancedFriendsGameInstance.h"
#include "Neuron.h"
#include "MyUserWidgetNeuron.h"
#include "XPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MULTIUSERTOOL_API AXPlayerController : public APlayerController
{
	GENERATED_BODY()
private:
	FHttpModule* Http;

	FString AuthorizationHeader = TEXT("Authorization");
	void SetAuthorizationHash(FString Hash, TSharedRef<IHttpRequest>& Request);

	TSharedRef<IHttpRequest> RequestWithRoute(FString Subroute);
	void SetRequestHeaders(TSharedRef<IHttpRequest>& Request);

	TSharedRef<IHttpRequest> GetRequest(FString Subroute);
	TSharedRef<IHttpRequest> PostRequest(FString Subroute, FString ContentJsonString);
	void Send(TSharedRef<IHttpRequest>& Request);

	bool ResponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful);

	/*Our Neuron UI Reference*/
	class UMyUserWidgetNeuron* NeuronWidget;
	class UMyUserWidgetNeuronInfo* NeuronWidgetInfo;

protected:

	virtual void BeginPlay() override;

	/*Property which is pointing to our Widget Blueprint in order to instantiate it using c++*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Neuron UI")
	TSubclassOf<class UMyUserWidgetNeuron> NeuronWidgetBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Neuron UI")
	TSubclassOf<class UMyUserWidgetNeuronInfo> NeuronWidgetInfoBP;

public:
	//player controller y gameinstance
	FString ApiBaseUrl = "http://localhost:8080/multiuser/";
	/** REST RegisterUrl */
	FString GetMessagesUrl = "messages/session.php";
	/** REST RegisterUrl */
	FString PostNeuronInfo = "neuron/submit_information.php";

	bool bShowingNeuronWidget = false;
	bool bShowingNeuronWidgetInfo = false;

	AXPlayerController(const FObjectInitializer& ObjectInitializer);

	/**REST get all Messages for a session */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "REST get all Messages", Keywords = "REST get all Messages"), Category = "REST")
		void GetAllMessages();

	/** Enhace funcion rpc to do in server*/
	UFUNCTION(Server, Reliable, WithValidation)
	void EnhanceNeuron(int32 idneuron);
	
	/** HideEnhacedNeurons funcion rpc to do in server*/
	UFUNCTION(Server, Reliable, WithValidation)
	void HideEnhacedNeurons();
	
	/** HideNotEnhacedNeurons funcion rpc to do in server*/
	UFUNCTION(Server, Reliable, WithValidation)
	void HideNotEnhacedNeurons();

	/** UnHideEnhacedNeurons funcion rpc to do in server*/
	UFUNCTION(Server, Reliable, WithValidation)
	void UnHideEnhacedNeurons();

	/** UnHideNotEnhacedNeurons funcion rpc to do in server*/
	UFUNCTION(Server, Reliable, WithValidation)
	void UnHideNotEnhacedNeurons();

	/** UnHideNotEnhacedNeurons funcion rpc to do in server*/
	UFUNCTION(Server, Reliable, WithValidation)
	void SetInfoNeuron(int32 idneuron, int32 idInfo, const FString& NeuronInfo);

	/** UnHideNotEnhacedNeurons funcion rpc to do in server*/
	UFUNCTION(Server, Reliable, WithValidation)
	void PlayReplay();

	UFUNCTION(BlueprintCallable, Category = NeuronUI)
	void ToggleNeuronWidget();

	UFUNCTION(BlueprintCallable, Category = NeuronUI)
	void ToggleNeuronInfoWidget(int32 idneuron, int32 idInfo, FString NeuronInfo);

	void GetAllMessagesResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
	void SetInfoNeuronResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
