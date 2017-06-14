// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	C++ class header boilerplate exported from UnrealHeaderTool.
	This is automatically generated by the tools.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

PRAGMA_DISABLE_DEPRECATION_WARNINGS
struct FSChatMsg;
#ifdef MULTIUSERTOOL_XPlayerState_generated_h
#error "XPlayerState.generated.h already included, missing '#pragma once' in XPlayerState.h"
#endif
#define MULTIUSERTOOL_XPlayerState_generated_h

#define MultiUserToolrotoNew_4_15_Source_MultiUserTool_XPlayerState_h_15_RPC_WRAPPERS \
	virtual bool UserChatP_Validate(FSChatMsg const& , FText const& ); \
	virtual void UserChatP_Implementation(FSChatMsg const& newmessage, FText const& userDestination); \
	virtual bool UserChat_Validate(FSChatMsg const& ); \
	virtual void UserChat_Implementation(FSChatMsg const& newmessage); \
	virtual bool UserChatRPC_Validate(FSChatMsg const& , FText const& , bool ); \
	virtual void UserChatRPC_Implementation(FSChatMsg const& newmessage, FText const& userDestination, bool isPrivate); \
 \
	DECLARE_FUNCTION(execUserChatP) \
	{ \
		P_GET_STRUCT(FSChatMsg,Z_Param_newmessage); \
		P_GET_PROPERTY(UTextProperty,Z_Param_userDestination); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		if (!this->UserChatP_Validate(Z_Param_newmessage,Z_Param_userDestination)) \
		{ \
			RPC_ValidateFailed(TEXT("UserChatP_Validate")); \
			return; \
		} \
		this->UserChatP_Implementation(Z_Param_newmessage,Z_Param_userDestination); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execUserChat) \
	{ \
		P_GET_STRUCT(FSChatMsg,Z_Param_newmessage); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		if (!this->UserChat_Validate(Z_Param_newmessage)) \
		{ \
			RPC_ValidateFailed(TEXT("UserChat_Validate")); \
			return; \
		} \
		this->UserChat_Implementation(Z_Param_newmessage); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execUserChatRPC) \
	{ \
		P_GET_STRUCT(FSChatMsg,Z_Param_newmessage); \
		P_GET_PROPERTY(UTextProperty,Z_Param_userDestination); \
		P_GET_UBOOL(Z_Param_isPrivate); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		if (!this->UserChatRPC_Validate(Z_Param_newmessage,Z_Param_userDestination,Z_Param_isPrivate)) \
		{ \
			RPC_ValidateFailed(TEXT("UserChatRPC_Validate")); \
			return; \
		} \
		this->UserChatRPC_Implementation(Z_Param_newmessage,Z_Param_userDestination,Z_Param_isPrivate); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetTeamNumber) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(int32*)Z_Param__Result=this->GetTeamNumber(); \
		P_NATIVE_END; \
	}


#define MultiUserToolrotoNew_4_15_Source_MultiUserTool_XPlayerState_h_15_RPC_WRAPPERS_NO_PURE_DECLS \
	virtual bool UserChatP_Validate(FSChatMsg const& , FText const& ); \
	virtual void UserChatP_Implementation(FSChatMsg const& newmessage, FText const& userDestination); \
	virtual bool UserChat_Validate(FSChatMsg const& ); \
	virtual void UserChat_Implementation(FSChatMsg const& newmessage); \
	virtual bool UserChatRPC_Validate(FSChatMsg const& , FText const& , bool ); \
	virtual void UserChatRPC_Implementation(FSChatMsg const& newmessage, FText const& userDestination, bool isPrivate); \
 \
	DECLARE_FUNCTION(execUserChatP) \
	{ \
		P_GET_STRUCT(FSChatMsg,Z_Param_newmessage); \
		P_GET_PROPERTY(UTextProperty,Z_Param_userDestination); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		if (!this->UserChatP_Validate(Z_Param_newmessage,Z_Param_userDestination)) \
		{ \
			RPC_ValidateFailed(TEXT("UserChatP_Validate")); \
			return; \
		} \
		this->UserChatP_Implementation(Z_Param_newmessage,Z_Param_userDestination); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execUserChat) \
	{ \
		P_GET_STRUCT(FSChatMsg,Z_Param_newmessage); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		if (!this->UserChat_Validate(Z_Param_newmessage)) \
		{ \
			RPC_ValidateFailed(TEXT("UserChat_Validate")); \
			return; \
		} \
		this->UserChat_Implementation(Z_Param_newmessage); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execUserChatRPC) \
	{ \
		P_GET_STRUCT(FSChatMsg,Z_Param_newmessage); \
		P_GET_PROPERTY(UTextProperty,Z_Param_userDestination); \
		P_GET_UBOOL(Z_Param_isPrivate); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		if (!this->UserChatRPC_Validate(Z_Param_newmessage,Z_Param_userDestination,Z_Param_isPrivate)) \
		{ \
			RPC_ValidateFailed(TEXT("UserChatRPC_Validate")); \
			return; \
		} \
		this->UserChatRPC_Implementation(Z_Param_newmessage,Z_Param_userDestination,Z_Param_isPrivate); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetTeamNumber) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(int32*)Z_Param__Result=this->GetTeamNumber(); \
		P_NATIVE_END; \
	}


#define MultiUserToolrotoNew_4_15_Source_MultiUserTool_XPlayerState_h_15_EVENT_PARMS \
	struct XPlayerState_eventUserChat_Parms \
	{ \
		FSChatMsg newmessage; \
	}; \
	struct XPlayerState_eventUserChatP_Parms \
	{ \
		FSChatMsg newmessage; \
		FText userDestination; \
	}; \
	struct XPlayerState_eventUserChatRPC_Parms \
	{ \
		FSChatMsg newmessage; \
		FText userDestination; \
		bool isPrivate; \
	};


extern MULTIUSERTOOL_API  FName MULTIUSERTOOL_UserChat;
extern MULTIUSERTOOL_API  FName MULTIUSERTOOL_UserChatP;
extern MULTIUSERTOOL_API  FName MULTIUSERTOOL_UserChatRPC;
#define MultiUserToolrotoNew_4_15_Source_MultiUserTool_XPlayerState_h_15_CALLBACK_WRAPPERS
#define MultiUserToolrotoNew_4_15_Source_MultiUserTool_XPlayerState_h_15_INCLASS_NO_PURE_DECLS \
	private: \
	static void StaticRegisterNativesAXPlayerState(); \
	friend MULTIUSERTOOL_API class UClass* Z_Construct_UClass_AXPlayerState(); \
	public: \
	DECLARE_CLASS(AXPlayerState, APlayerState, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/MultiUserTool"), NO_API) \
	DECLARE_SERIALIZER(AXPlayerState) \
	/** Indicates whether the class is compiled into the engine */ \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC}; \
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


#define MultiUserToolrotoNew_4_15_Source_MultiUserTool_XPlayerState_h_15_INCLASS \
	private: \
	static void StaticRegisterNativesAXPlayerState(); \
	friend MULTIUSERTOOL_API class UClass* Z_Construct_UClass_AXPlayerState(); \
	public: \
	DECLARE_CLASS(AXPlayerState, APlayerState, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/MultiUserTool"), NO_API) \
	DECLARE_SERIALIZER(AXPlayerState) \
	/** Indicates whether the class is compiled into the engine */ \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC}; \
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


#define MultiUserToolrotoNew_4_15_Source_MultiUserTool_XPlayerState_h_15_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API AXPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AXPlayerState) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AXPlayerState); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AXPlayerState); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AXPlayerState(AXPlayerState&&); \
	NO_API AXPlayerState(const AXPlayerState&); \
public:


#define MultiUserToolrotoNew_4_15_Source_MultiUserTool_XPlayerState_h_15_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AXPlayerState(AXPlayerState&&); \
	NO_API AXPlayerState(const AXPlayerState&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AXPlayerState); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AXPlayerState); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AXPlayerState)


#define MultiUserToolrotoNew_4_15_Source_MultiUserTool_XPlayerState_h_15_PRIVATE_PROPERTY_OFFSET
#define MultiUserToolrotoNew_4_15_Source_MultiUserTool_XPlayerState_h_12_PROLOG \
	MultiUserToolrotoNew_4_15_Source_MultiUserTool_XPlayerState_h_15_EVENT_PARMS


#define MultiUserToolrotoNew_4_15_Source_MultiUserTool_XPlayerState_h_15_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	MultiUserToolrotoNew_4_15_Source_MultiUserTool_XPlayerState_h_15_PRIVATE_PROPERTY_OFFSET \
	MultiUserToolrotoNew_4_15_Source_MultiUserTool_XPlayerState_h_15_RPC_WRAPPERS \
	MultiUserToolrotoNew_4_15_Source_MultiUserTool_XPlayerState_h_15_CALLBACK_WRAPPERS \
	MultiUserToolrotoNew_4_15_Source_MultiUserTool_XPlayerState_h_15_INCLASS \
	MultiUserToolrotoNew_4_15_Source_MultiUserTool_XPlayerState_h_15_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define MultiUserToolrotoNew_4_15_Source_MultiUserTool_XPlayerState_h_15_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	MultiUserToolrotoNew_4_15_Source_MultiUserTool_XPlayerState_h_15_PRIVATE_PROPERTY_OFFSET \
	MultiUserToolrotoNew_4_15_Source_MultiUserTool_XPlayerState_h_15_RPC_WRAPPERS_NO_PURE_DECLS \
	MultiUserToolrotoNew_4_15_Source_MultiUserTool_XPlayerState_h_15_CALLBACK_WRAPPERS \
	MultiUserToolrotoNew_4_15_Source_MultiUserTool_XPlayerState_h_15_INCLASS_NO_PURE_DECLS \
	MultiUserToolrotoNew_4_15_Source_MultiUserTool_XPlayerState_h_15_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID MultiUserToolrotoNew_4_15_Source_MultiUserTool_XPlayerState_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
