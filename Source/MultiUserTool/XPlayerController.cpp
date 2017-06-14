// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiUserTool.h"
#include "XPlayerController.h"
#include "NavigatorHUD.h"
#include "MyActorNotification.h"
#include "SlateChat.h"
#include "MultiUserToolGameMode.h"


AXPlayerController::AXPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	/* Make sure the PawnClass is Replicated */
	bReplicates = true;
	bReplicateMovement = true;
	bEnableMouseOverEvents = true;
	Http = &FHttpModule::Get();
}

void AXPlayerController::BeginPlay()
{
	Super::BeginPlay();
	GetAllMessages();
	
	if (NeuronWidgetBP)
	{
		//Creating our widget and adding it to our viewport
		NeuronWidget = CreateWidget<UMyUserWidgetNeuron>(this, NeuronWidgetBP);
		NeuronWidget->AddToViewport();
		NeuronWidget->SetVisibility(ESlateVisibility::Hidden); // Set it to hidden so its not open on spawn.
	}
}

TSharedRef<IHttpRequest> AXPlayerController::RequestWithRoute(FString Subroute) {
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->SetURL(ApiBaseUrl + Subroute);
	SetRequestHeaders(Request);
	return Request;
}

void AXPlayerController::SetRequestHeaders(TSharedRef<IHttpRequest>& Request) {
	Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accepts"), TEXT("application/json"));
}

TSharedRef<IHttpRequest> AXPlayerController::GetRequest(FString Subroute) {
	TSharedRef<IHttpRequest> Request = RequestWithRoute(Subroute);
	Request->SetVerb("GET");
	return Request;
}

TSharedRef<IHttpRequest> AXPlayerController::PostRequest(FString Subroute, FString ContentJsonString) {
	TSharedRef<IHttpRequest> Request = RequestWithRoute(Subroute);
	Request->SetVerb("POST");
	Request->SetContentAsString(ContentJsonString);

	return Request;
}

void AXPlayerController::Send(TSharedRef<IHttpRequest>& Request) {
	Request->ProcessRequest();
}

bool AXPlayerController::ResponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful) {
	if (!bWasSuccessful || !Response.IsValid()) return false;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode())) return true;
	else {
		UE_LOG(LogTemp, Warning, TEXT("Http Response returned error code: %d"), Response->GetResponseCode());
		return false;
	}
}

void AXPlayerController::SetAuthorizationHash(FString Hash, TSharedRef<IHttpRequest>& Request) {
	Request->SetHeader(AuthorizationHeader, Hash);
}

/***************************************************************************************************************************************/

void AXPlayerController::GetAllMessages() {
	UMyAdvancedFriendsGameInstance* GI = Cast<UMyAdvancedFriendsGameInstance>(GetGameInstance());
	FString sessionId = TEXT("");
	if (GI) sessionId = GI->sessionId;
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField("session", sessionId);

	FString ContentJsonString;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&ContentJsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	TSharedRef<IHttpRequest> Request = PostRequest(GetMessagesUrl, ContentJsonString);
	Request->OnProcessRequestComplete().BindUObject(this, &AXPlayerController::GetAllMessagesResponse);
	Send(Request);
}

void AXPlayerController::GetAllMessagesResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
	//if (!ResponseIsValid(Response, bWasSuccessful)) return;

	FString JsonString = Response->GetContentAsString();
	UE_LOG(LogTemp, Warning, TEXT("json: %s"), *JsonString);
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(JsonString);
	if (FJsonSerializer::Deserialize(Reader, JsonObject) &&
		JsonObject.IsValid())
	{
		int32 ok = JsonObject->GetIntegerField(TEXT("ok"));
		if (ok == 0) {
			UE_LOG(LogTemp, Warning, TEXT("is ok: %s"), TEXT("no"));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("is ok: %s"), TEXT("yes"));
			TArray< TSharedPtr< FJsonValue > > messages;
			messages = JsonObject->GetArrayField("results");
			TArray<TSharedPtr<FSChatMsg>> messagesArray;
			for (int i = 0; i < messages.Num(); i++)
			{
				auto res = messages[i]->AsObject();
				int32 ty = res->GetIntegerField("type");
				FString us = res->GetStringField("user");
				FString te = res->GetStringField("text");
				FString ti = res->GetStringField("time");
				FString Receiver = res->GetStringField("receiver");
				UMyAdvancedFriendsGameInstance*	GI = Cast<UMyAdvancedFriendsGameInstance>(GetGameInstance());
				UE_LOG(LogTemp, Warning, TEXT("receiver: %s"), *Receiver);
				UE_LOG(LogTemp, Warning, TEXT("user: %s"), *GI->userName);

				if (ty < 3 || (ty == 3 && Receiver.Compare(GI->userName) == 0)) {
					int32 index = messagesArray.Add(MakeShareable(new FSChatMsg())); // add a new message to the chatbox array
					if (messagesArray[index].IsValid())
					{
						messagesArray[index]->Init(ty, FText::FromString(us), FText::FromString(te)); // intiate our new message with the passed message
						messagesArray[index]->SetTime(FText::FromString(ti), FPlatformTime::Seconds()); // Comment this line to remove timestamps or replace FPlatformTime::Seconds() with 0 to slow decay the messages
						messagesArray.Shrink();
					}
				}
			}
			ANavigatorHUD* MyHud = Cast<ANavigatorHUD>(GetHUD());
			if (MyHud && MyHud->MyUIWidget.IsValid())
				MyHud->MyUIWidget->SetOldMessages(messagesArray);
		}
	}
}

void AXPlayerController::SetInfoNeuronResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {

	FString JsonString = Response->GetContentAsString();
	UE_LOG(LogTemp, Warning, TEXT("json: %s"), *JsonString);
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(JsonString);
	if (FJsonSerializer::Deserialize(Reader, JsonObject) &&
		JsonObject.IsValid())
	{
		int32 ok = JsonObject->GetIntegerField(TEXT("ok"));
		if (ok == 0) {
			UE_LOG(LogTemp, Warning, TEXT("SetInfoNeuronResponse is ok: %s"), TEXT("no"));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("SetInfoNeuronResponse is ok: %s"), TEXT("yes"));
		}
	}
}

bool AXPlayerController::EnhanceNeuron_Validate(int32 idneuron)
{
	return true;
}

void AXPlayerController::EnhanceNeuron_Implementation(int32 idneuron)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("Neuron to enhace %d"), idneuron));
	}
	for (TObjectIterator<ANeuron> Itr; Itr; ++Itr){
		ANeuron* CurrentNeuron = Cast<ANeuron>(*Itr);
		if (idneuron == CurrentNeuron->idneuron) {
			CurrentNeuron->Enhance();
		}
		/*if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("List Name %d"), CurrentNeuron->idneuron));
		}*/
	}
}

bool AXPlayerController::HideEnhacedNeurons_Validate()
{
	return true;
}

void AXPlayerController::HideEnhacedNeurons_Implementation()
{
	for (TObjectIterator<ANeuron> Itr; Itr; ++Itr) {
		ANeuron* CurrentNeuron = Cast<ANeuron>(*Itr);
		if (CurrentNeuron->isEnhaced) {
			CurrentNeuron->SetActorHiddenInGame(true);
		}
	}
}

bool AXPlayerController::HideNotEnhacedNeurons_Validate()
{
	return true;
}

void AXPlayerController::HideNotEnhacedNeurons_Implementation()
{
	for (TObjectIterator<ANeuron> Itr; Itr; ++Itr) {
		ANeuron* CurrentNeuron = Cast<ANeuron>(*Itr);
		if (!CurrentNeuron->isEnhaced) {
			CurrentNeuron->SetActorHiddenInGame(true);
		}
	}
}

bool AXPlayerController::UnHideEnhacedNeurons_Validate()
{
	return true;
}

void AXPlayerController::UnHideEnhacedNeurons_Implementation()
{
	for (TObjectIterator<ANeuron> Itr; Itr; ++Itr) {
		ANeuron* CurrentNeuron = Cast<ANeuron>(*Itr);
		if (CurrentNeuron->isEnhaced) {
			CurrentNeuron->SetActorHiddenInGame(false);
		}
	}
}

bool AXPlayerController::UnHideNotEnhacedNeurons_Validate()
{
	return true;
}

void AXPlayerController::UnHideNotEnhacedNeurons_Implementation()
{
	for (TObjectIterator<ANeuron> Itr; Itr; ++Itr) {
		ANeuron* CurrentNeuron = Cast<ANeuron>(*Itr);
		if (!CurrentNeuron->isEnhaced) {
			CurrentNeuron->SetActorHiddenInGame(false);
		}
	}
}


bool AXPlayerController::SetInfoNeuron_Validate(int32 idneuron, int32 idInfo, const FString& NeuronInfo)
{
	return true;
}

void AXPlayerController::SetInfoNeuron_Implementation(int32 idneuron, int32 idInfo, const FString& NeuronInfo)
{
	for (TObjectIterator<AMyActorNotification> Itr; Itr; ++Itr) {
		AMyActorNotification* CurrentNeuron = Cast<AMyActorNotification>(*Itr);
		if (CurrentNeuron->idneuron == idneuron && CurrentNeuron->idInfo == idInfo) {
			CurrentNeuron->NeuronInfo = NeuronInfo;
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("setinfo"));
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("WidgetInfo %d"), idInfo));
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("WidgetInfo %s"), *NeuronInfo));
		}
	}
	ToggleNeuronInfoWidget(idneuron, idInfo, NeuronInfo);

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField("info", NeuronInfo);
	JsonObject->SetStringField("id", FString::Printf(TEXT("%d"), idInfo));
	JsonObject->SetStringField("idneuron", FString::Printf(TEXT("%d"), idneuron));

	FString ContentJsonString;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&ContentJsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	TSharedRef<IHttpRequest> Request = PostRequest(PostNeuronInfo, ContentJsonString);
	Request->OnProcessRequestComplete().BindUObject(this, &AXPlayerController::SetInfoNeuronResponse);
	Send(Request);
}

bool AXPlayerController::PlayReplay_Validate()
{
	return true;
}

void AXPlayerController::PlayReplay_Implementation()
{
	AMultiUserToolGameMode* GM = (AMultiUserToolGameMode*)GetWorld()->GetAuthGameMode();
	GM->PlayReplay();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("PlayerC playreplay"));
}

void AXPlayerController::ToggleNeuronWidget()
{
	
	if (NeuronWidgetBP && !NeuronWidget)
	{
		//Creating our widget and adding it to our viewport
		NeuronWidget = CreateWidget<UMyUserWidgetNeuron>(this, NeuronWidgetBP);
		NeuronWidget->AddToViewport();
		NeuronWidget->SetVisibility(ESlateVisibility::Hidden); // Set it to hidden so its not open on spawn.
	}
	if (bShowingNeuronWidget)
		bShowingNeuronWidget = false;
	else
		bShowingNeuronWidget = true;

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("bShowingNeuronWidget %s"), bShowingNeuronWidget? TEXT("true"): TEXT("false")));

	if (NeuronWidget)
	{
		if (bShowingNeuronWidget)
		{
			NeuronWidget->SetVisibility(ESlateVisibility::Visible);
		}
		else {
			NeuronWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AXPlayerController::ToggleNeuronInfoWidget(int32 idneuron, int32 idInfo, FString NeuronInfo)
{

	if (NeuronWidgetInfoBP && !NeuronWidgetInfo)
	{
		//Creating our widget and adding it to our viewport
		NeuronWidgetInfo = CreateWidget<UMyUserWidgetNeuronInfo>(this, NeuronWidgetInfoBP);
		NeuronWidgetInfo->AddToViewport();
		NeuronWidgetInfo->SetVisibility(ESlateVisibility::Hidden); // Set it to hidden so its not open on spawn.
	}
	if (bShowingNeuronWidgetInfo)
		bShowingNeuronWidgetInfo = false;
	else
		bShowingNeuronWidgetInfo = true;
		
	if (NeuronWidgetInfo)
	{
		if (bShowingNeuronWidgetInfo)
		{
			NeuronWidgetInfo->currentidneuron = idneuron;
			NeuronWidgetInfo->currentIdActorNotification = idInfo;
			NeuronWidgetInfo->CurrentNeuronInfo = NeuronInfo;
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("NeuronWidgetInfo %d"), idneuron));
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("NeuronWidgetInfo %d"), idInfo));
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("NeuronWidgetInfo %s"), *NeuronInfo));

			NeuronWidgetInfo->SetVisibility(ESlateVisibility::Visible);
		}
		else {
			NeuronWidgetInfo->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}


