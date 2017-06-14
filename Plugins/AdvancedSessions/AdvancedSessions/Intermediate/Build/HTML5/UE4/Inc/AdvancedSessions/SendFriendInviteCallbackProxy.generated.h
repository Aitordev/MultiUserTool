// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	C++ class header boilerplate exported from UnrealHeaderTool.
	This is automatically generated by the tools.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UObject;
class APlayerController;
struct FBPUniqueNetId;
class USendFriendInviteCallbackProxy;
#ifdef ADVANCEDSESSIONS_SendFriendInviteCallbackProxy_generated_h
#error "SendFriendInviteCallbackProxy.generated.h already included, missing '#pragma once' in SendFriendInviteCallbackProxy.h"
#endif
#define ADVANCEDSESSIONS_SendFriendInviteCallbackProxy_generated_h

#define MultiUserToolrotoNew_4_15_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_SendFriendInviteCallbackProxy_h_10_DELEGATE \
static inline void FBlueprintSendFriendInviteDelegate_DelegateWrapper(const FMulticastScriptDelegate& BlueprintSendFriendInviteDelegate) \
{ \
	BlueprintSendFriendInviteDelegate.ProcessMulticastDelegate<UObject>(NULL); \
}




#define MultiUserToolrotoNew_4_15_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_SendFriendInviteCallbackProxy_h_15_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execSendFriendInvite) \
	{ \
		P_GET_OBJECT(UObject,Z_Param_WorldContextObject); \
		P_GET_OBJECT(APlayerController,Z_Param_PlayerController); \
		P_GET_STRUCT_REF(FBPUniqueNetId,Z_Param_Out_UniqueNetIDInvited); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(USendFriendInviteCallbackProxy**)Z_Param__Result=USendFriendInviteCallbackProxy::SendFriendInvite(Z_Param_WorldContextObject,Z_Param_PlayerController,Z_Param_Out_UniqueNetIDInvited); \
		P_NATIVE_END; \
	}


#define MultiUserToolrotoNew_4_15_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_SendFriendInviteCallbackProxy_h_15_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execSendFriendInvite) \
	{ \
		P_GET_OBJECT(UObject,Z_Param_WorldContextObject); \
		P_GET_OBJECT(APlayerController,Z_Param_PlayerController); \
		P_GET_STRUCT_REF(FBPUniqueNetId,Z_Param_Out_UniqueNetIDInvited); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(USendFriendInviteCallbackProxy**)Z_Param__Result=USendFriendInviteCallbackProxy::SendFriendInvite(Z_Param_WorldContextObject,Z_Param_PlayerController,Z_Param_Out_UniqueNetIDInvited); \
		P_NATIVE_END; \
	}


#define MultiUserToolrotoNew_4_15_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_SendFriendInviteCallbackProxy_h_15_INCLASS_NO_PURE_DECLS \
	private: \
	static void StaticRegisterNativesUSendFriendInviteCallbackProxy(); \
	friend ADVANCEDSESSIONS_API class UClass* Z_Construct_UClass_USendFriendInviteCallbackProxy(); \
	public: \
	DECLARE_CLASS(USendFriendInviteCallbackProxy, UOnlineBlueprintCallProxyBase, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/AdvancedSessions"), ADVANCEDSESSIONS_API) \
	DECLARE_SERIALIZER(USendFriendInviteCallbackProxy) \
	/** Indicates whether the class is compiled into the engine */ \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define MultiUserToolrotoNew_4_15_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_SendFriendInviteCallbackProxy_h_15_INCLASS \
	private: \
	static void StaticRegisterNativesUSendFriendInviteCallbackProxy(); \
	friend ADVANCEDSESSIONS_API class UClass* Z_Construct_UClass_USendFriendInviteCallbackProxy(); \
	public: \
	DECLARE_CLASS(USendFriendInviteCallbackProxy, UOnlineBlueprintCallProxyBase, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/AdvancedSessions"), ADVANCEDSESSIONS_API) \
	DECLARE_SERIALIZER(USendFriendInviteCallbackProxy) \
	/** Indicates whether the class is compiled into the engine */ \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define MultiUserToolrotoNew_4_15_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_SendFriendInviteCallbackProxy_h_15_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	ADVANCEDSESSIONS_API USendFriendInviteCallbackProxy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(USendFriendInviteCallbackProxy) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(ADVANCEDSESSIONS_API, USendFriendInviteCallbackProxy); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(USendFriendInviteCallbackProxy); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	ADVANCEDSESSIONS_API USendFriendInviteCallbackProxy(USendFriendInviteCallbackProxy&&); \
	ADVANCEDSESSIONS_API USendFriendInviteCallbackProxy(const USendFriendInviteCallbackProxy&); \
public:


#define MultiUserToolrotoNew_4_15_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_SendFriendInviteCallbackProxy_h_15_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	ADVANCEDSESSIONS_API USendFriendInviteCallbackProxy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	ADVANCEDSESSIONS_API USendFriendInviteCallbackProxy(USendFriendInviteCallbackProxy&&); \
	ADVANCEDSESSIONS_API USendFriendInviteCallbackProxy(const USendFriendInviteCallbackProxy&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(ADVANCEDSESSIONS_API, USendFriendInviteCallbackProxy); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(USendFriendInviteCallbackProxy); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(USendFriendInviteCallbackProxy)


#define MultiUserToolrotoNew_4_15_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_SendFriendInviteCallbackProxy_h_15_PRIVATE_PROPERTY_OFFSET
#define MultiUserToolrotoNew_4_15_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_SendFriendInviteCallbackProxy_h_12_PROLOG
#define MultiUserToolrotoNew_4_15_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_SendFriendInviteCallbackProxy_h_15_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	MultiUserToolrotoNew_4_15_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_SendFriendInviteCallbackProxy_h_15_PRIVATE_PROPERTY_OFFSET \
	MultiUserToolrotoNew_4_15_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_SendFriendInviteCallbackProxy_h_15_RPC_WRAPPERS \
	MultiUserToolrotoNew_4_15_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_SendFriendInviteCallbackProxy_h_15_INCLASS \
	MultiUserToolrotoNew_4_15_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_SendFriendInviteCallbackProxy_h_15_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define MultiUserToolrotoNew_4_15_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_SendFriendInviteCallbackProxy_h_15_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	MultiUserToolrotoNew_4_15_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_SendFriendInviteCallbackProxy_h_15_PRIVATE_PROPERTY_OFFSET \
	MultiUserToolrotoNew_4_15_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_SendFriendInviteCallbackProxy_h_15_RPC_WRAPPERS_NO_PURE_DECLS \
	MultiUserToolrotoNew_4_15_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_SendFriendInviteCallbackProxy_h_15_INCLASS_NO_PURE_DECLS \
	MultiUserToolrotoNew_4_15_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_SendFriendInviteCallbackProxy_h_15_ENHANCED_CONSTRUCTORS \
static_assert(false, "Unknown access specifier for GENERATED_BODY() macro in class SendFriendInviteCallbackProxy."); \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID MultiUserToolrotoNew_4_15_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_SendFriendInviteCallbackProxy_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
