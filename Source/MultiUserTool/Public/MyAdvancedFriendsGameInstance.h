// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AdvancedFriendsGameInstance.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Json.h"
#include "MyAdvancedFriendsGameInstance.generated.h"

USTRUCT()
struct FSChatMsg // Struct to hold the message data to be passed between classes
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY() // UProperty means this variable will be replicated
		int32 Type;

	UPROPERTY()
		FText Username;

	UPROPERTY()
		FText Text;

	FText Timestamp; // Dont replicate time because we can set it locally once we receive the struct

	double Created;

	void Init(int32 NewType, FText NewUsername, FText NewText) // Assign only the vars we wish to replicate
	{
		Type = NewType;
		Username = NewUsername;
		Text = NewText;
	}
	void SetTime(FText NewTimestamp, double NewCreated)
	{
		Timestamp = NewTimestamp;
		Created = NewCreated;
	}
	void Destroy()
	{
		Type = NULL;
		Username.GetEmpty();
		Text.GetEmpty();
		Timestamp.GetEmpty();
		Created = NULL;
	}
};

UCLASS()
class MULTIUSERTOOL_API UMyAdvancedFriendsGameInstance : public UAdvancedFriendsGameInstance
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

public:
	//player controller y gameinstance
	FString ApiBaseUrl = "http://localhost:8080/multiuser/";
	/** REST LoginUrl */
	FString LoginUrl = "tokens/login.php";
	/** REST RegisterUrl */
	FString RegisterUrl = "users/new.php";
	/** REST RegisterUrl */
	FString NewMessageUrl = "messages/submit.php";
		
	UMyAdvancedFriendsGameInstance(const class FObjectInitializer& ObjectInitializer);

	/** User id */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = User)
	FString userId;

	/** User name */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = User)
	FString userName;

	/** User token */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = User)
	FString userToken;
	
	/** User is Logged */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = User)
	bool userIsLogged = false;
	
	/** User registration succes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = User)
	bool userIsRegistered = false;

	/** User registration succes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = User)
	bool IsGuest = false;

	/** User registration error msg */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = User)
	FString registrationFailed;

	/** Session id */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Session)
	FString sessionId;

	/** Md5 generated from string */
	UFUNCTION(BlueprintPure, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", DisplayName = "Get MD5 from string",
		CompactNodeTitle = "MD5", Keywords = "md5 hash string"), Category = Hash)
	static FString GetMD5Sum(FString InputText);

	/** Guest Login */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Guest Login", Keywords = "Guest login"), Category = "Guest")
	void GuestLogin();

	/** REST Login Request */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "REST login", Keywords = "REST login"), Category = "REST")
	void Login(FString email, FString pass);

	void LoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	/** REST Register Request */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "REST Register", Keywords = "REST register"), Category = "REST")
	void Register(FString userNameInput, FString userEmailInput, FString userPasswordInput, FString invitationCode);

	void RegisterResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
	/**REST put New Message on session */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "REST New Message", Keywords = "REST New Message"), Category = "REST")
	void NewMessage(const FSChatMsg& newmessage, const FText receiver);

	void NewMessageResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
