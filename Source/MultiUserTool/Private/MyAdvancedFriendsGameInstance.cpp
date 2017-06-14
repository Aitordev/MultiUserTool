// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiUserTool.h"
#include "MyAdvancedFriendsGameInstance.h"


UMyAdvancedFriendsGameInstance::UMyAdvancedFriendsGameInstance(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//When the object is constructed, Get the HTTP module
	Http = &FHttpModule::Get();

}

FString UMyAdvancedFriendsGameInstance::GetMD5Sum(FString InputText)
{
	return FMD5::HashAnsiString(*InputText);
}

void UMyAdvancedFriendsGameInstance::GuestLogin()
{
	userId = TEXT("guest");
	userName = TEXT("guest");
	userToken = TEXT("guest");
	userIsLogged = true;
	IsGuest = true;
}

TSharedRef<IHttpRequest> UMyAdvancedFriendsGameInstance::RequestWithRoute(FString Subroute) {
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->SetURL(ApiBaseUrl + Subroute);
	SetRequestHeaders(Request);
	return Request;
}

void UMyAdvancedFriendsGameInstance::SetRequestHeaders(TSharedRef<IHttpRequest>& Request) {
	Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accepts"), TEXT("application/json"));
}

TSharedRef<IHttpRequest> UMyAdvancedFriendsGameInstance::GetRequest(FString Subroute) {
	TSharedRef<IHttpRequest> Request = RequestWithRoute(Subroute);
	Request->SetVerb("GET");
	return Request;
}

TSharedRef<IHttpRequest> UMyAdvancedFriendsGameInstance::PostRequest(FString Subroute, FString ContentJsonString) {
	TSharedRef<IHttpRequest> Request = RequestWithRoute(Subroute);
	Request->SetVerb("POST");
	Request->SetContentAsString(ContentJsonString);
	
	return Request;
}

void UMyAdvancedFriendsGameInstance::Send(TSharedRef<IHttpRequest>& Request) {
	Request->ProcessRequest();
}

bool UMyAdvancedFriendsGameInstance::ResponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful) {
	if (!bWasSuccessful || !Response.IsValid()) return false;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode())) return true;
	else {
		UE_LOG(LogTemp, Warning, TEXT("Http Response returned error code: %d"), Response->GetResponseCode());
		return false;
	}
}

void UMyAdvancedFriendsGameInstance::SetAuthorizationHash(FString Hash, TSharedRef<IHttpRequest>& Request) {
	Request->SetHeader(AuthorizationHeader, Hash);
}



/***************************************************************************************************************************************/



void UMyAdvancedFriendsGameInstance::Login(FString userEmail, FString userPassword) {
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField("email", userEmail);
	JsonObject->SetStringField("password", userPassword);
	
	FString ContentJsonString;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&ContentJsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	
	TSharedRef<IHttpRequest> Request = PostRequest(LoginUrl, ContentJsonString);
	Request->OnProcessRequestComplete().BindUObject(this, &UMyAdvancedFriendsGameInstance::LoginResponse);
	Send(Request);
}

void UMyAdvancedFriendsGameInstance::LoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
	//if (!ResponseIsValid(Response, bWasSuccessful)) return;
	
	FString JsonString = Response->GetContentAsString();
	UE_LOG(LogTemp, Warning, TEXT("json: %s"), *JsonString);
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(JsonString);
	if (FJsonSerializer::Deserialize(Reader, JsonObject) &&
		JsonObject.IsValid())
	{
		userId = JsonObject->GetStringField(TEXT("email"));
		userName = JsonObject->GetStringField(TEXT("name"));
		userToken = JsonObject->GetStringField(TEXT("hash"));
		if (userId.Compare(TEXT("none")) == 0) {
			UE_LOG(LogTemp, Warning, TEXT("is logged: %s"), TEXT("no"));
			userIsLogged = false;
		}
		else{
			UE_LOG(LogTemp, Warning, TEXT("is logged: %s"), TEXT("yes"));
			userIsLogged = true;
		}
	}
}

void UMyAdvancedFriendsGameInstance::Register(FString userNameInput, FString userEmailInput, FString userPasswordInput, FString invitationCode) {
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField("email", userEmailInput);
	JsonObject->SetStringField("password", userPasswordInput);
	JsonObject->SetStringField("name", userNameInput);
	JsonObject->SetStringField("code", invitationCode);

	FString ContentJsonString;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&ContentJsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	TSharedRef<IHttpRequest> Request = PostRequest(RegisterUrl, ContentJsonString);
	Request->OnProcessRequestComplete().BindUObject(this, &UMyAdvancedFriendsGameInstance::RegisterResponse);
	Send(Request);
}

void UMyAdvancedFriendsGameInstance::RegisterResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
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
			UE_LOG(LogTemp, Warning, TEXT("is registered: %s"), TEXT("no"));
			userIsRegistered = false;
			registrationFailed = JsonObject->GetStringField(TEXT("failure"));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("is registered: %s"), TEXT("yes"));
			userIsRegistered = true;
			userId = JsonObject->GetStringField(TEXT("email"));
			userName = JsonObject->GetStringField(TEXT("name"));
			userToken = JsonObject->GetStringField(TEXT("hash"));
		}
	}
}

void UMyAdvancedFriendsGameInstance::NewMessage(const FSChatMsg& newmessage, const FText receiver) {
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	int32 Year, Month, Day, DayOfWeek, Hour, Minute, Second, Millisecond; // set the timestamp and decay timer
	FPlatformTime::SystemTime(Year, Month, DayOfWeek, Day, Hour, Minute, Second, Millisecond);
	FText Timestamp = FText::FromString(FString::Printf(TEXT("[ %02d:%02d:%02d ] "), Hour, Minute, Second));

	JsonObject->SetStringField("session", sessionId);
	JsonObject->SetNumberField("type", newmessage.Type);
	JsonObject->SetStringField("user", newmessage.Username.ToString());
	JsonObject->SetStringField("text", newmessage.Text.ToString());
	JsonObject->SetStringField("timestamp", Timestamp.ToString());
	JsonObject->SetStringField("receiver", receiver.ToString());

	FString ContentJsonString;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&ContentJsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	TSharedRef<IHttpRequest> Request = PostRequest(NewMessageUrl, ContentJsonString);
	Request->OnProcessRequestComplete().BindUObject(this, &UMyAdvancedFriendsGameInstance::NewMessageResponse);
	Send(Request);
}

void UMyAdvancedFriendsGameInstance::NewMessageResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
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
			UE_LOG(LogTemp, Warning, TEXT("NewMessageResponse is ok: %s"), TEXT("no"));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("NewMessageResponse is ok: %s"), TEXT("yes"));
		}
	}
}


