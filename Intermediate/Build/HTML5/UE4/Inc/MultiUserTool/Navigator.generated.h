// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	C++ class header boilerplate exported from UnrealHeaderTool.
	This is automatically generated by the tools.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

PRAGMA_DISABLE_DEPRECATION_WARNINGS
struct FRotator;
#ifdef MULTIUSERTOOL_Navigator_generated_h
#error "Navigator.generated.h already included, missing '#pragma once' in Navigator.h"
#endif
#define MULTIUSERTOOL_Navigator_generated_h

#define MultiUserToolrotoNew_4_15_Source_MultiUserTool_Navigator_h_38_RPC_WRAPPERS \
	virtual bool ServerMoveUpDown_Validate(float , FRotator ); \
	virtual void ServerMoveUpDown_Implementation(float Value, FRotator Rotation); \
	virtual bool ServerMoveRight_Validate(float , FRotator ); \
	virtual void ServerMoveRight_Implementation(float Value, FRotator Rotation); \
	virtual bool ServerMoveForward_Validate(float , FRotator ); \
	virtual void ServerMoveForward_Implementation(float Value, FRotator Rotation); \
 \
	DECLARE_FUNCTION(execOnRep_RotChange) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->OnRep_RotChange(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execOnRep_PosChange) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->OnRep_PosChange(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execServerMoveUpDown) \
	{ \
		P_GET_PROPERTY(UFloatProperty,Z_Param_Value); \
		P_GET_STRUCT(FRotator,Z_Param_Rotation); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		if (!this->ServerMoveUpDown_Validate(Z_Param_Value,Z_Param_Rotation)) \
		{ \
			RPC_ValidateFailed(TEXT("ServerMoveUpDown_Validate")); \
			return; \
		} \
		this->ServerMoveUpDown_Implementation(Z_Param_Value,Z_Param_Rotation); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execServerMoveRight) \
	{ \
		P_GET_PROPERTY(UFloatProperty,Z_Param_Value); \
		P_GET_STRUCT(FRotator,Z_Param_Rotation); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		if (!this->ServerMoveRight_Validate(Z_Param_Value,Z_Param_Rotation)) \
		{ \
			RPC_ValidateFailed(TEXT("ServerMoveRight_Validate")); \
			return; \
		} \
		this->ServerMoveRight_Implementation(Z_Param_Value,Z_Param_Rotation); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execServerMoveForward) \
	{ \
		P_GET_PROPERTY(UFloatProperty,Z_Param_Value); \
		P_GET_STRUCT(FRotator,Z_Param_Rotation); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		if (!this->ServerMoveForward_Validate(Z_Param_Value,Z_Param_Rotation)) \
		{ \
			RPC_ValidateFailed(TEXT("ServerMoveForward_Validate")); \
			return; \
		} \
		this->ServerMoveForward_Implementation(Z_Param_Value,Z_Param_Rotation); \
		P_NATIVE_END; \
	}


#define MultiUserToolrotoNew_4_15_Source_MultiUserTool_Navigator_h_38_RPC_WRAPPERS_NO_PURE_DECLS \
	virtual bool ServerMoveUpDown_Validate(float , FRotator ); \
	virtual void ServerMoveUpDown_Implementation(float Value, FRotator Rotation); \
	virtual bool ServerMoveRight_Validate(float , FRotator ); \
	virtual void ServerMoveRight_Implementation(float Value, FRotator Rotation); \
	virtual bool ServerMoveForward_Validate(float , FRotator ); \
	virtual void ServerMoveForward_Implementation(float Value, FRotator Rotation); \
 \
	DECLARE_FUNCTION(execOnRep_RotChange) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->OnRep_RotChange(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execOnRep_PosChange) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->OnRep_PosChange(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execServerMoveUpDown) \
	{ \
		P_GET_PROPERTY(UFloatProperty,Z_Param_Value); \
		P_GET_STRUCT(FRotator,Z_Param_Rotation); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		if (!this->ServerMoveUpDown_Validate(Z_Param_Value,Z_Param_Rotation)) \
		{ \
			RPC_ValidateFailed(TEXT("ServerMoveUpDown_Validate")); \
			return; \
		} \
		this->ServerMoveUpDown_Implementation(Z_Param_Value,Z_Param_Rotation); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execServerMoveRight) \
	{ \
		P_GET_PROPERTY(UFloatProperty,Z_Param_Value); \
		P_GET_STRUCT(FRotator,Z_Param_Rotation); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		if (!this->ServerMoveRight_Validate(Z_Param_Value,Z_Param_Rotation)) \
		{ \
			RPC_ValidateFailed(TEXT("ServerMoveRight_Validate")); \
			return; \
		} \
		this->ServerMoveRight_Implementation(Z_Param_Value,Z_Param_Rotation); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execServerMoveForward) \
	{ \
		P_GET_PROPERTY(UFloatProperty,Z_Param_Value); \
		P_GET_STRUCT(FRotator,Z_Param_Rotation); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		if (!this->ServerMoveForward_Validate(Z_Param_Value,Z_Param_Rotation)) \
		{ \
			RPC_ValidateFailed(TEXT("ServerMoveForward_Validate")); \
			return; \
		} \
		this->ServerMoveForward_Implementation(Z_Param_Value,Z_Param_Rotation); \
		P_NATIVE_END; \
	}


#define MultiUserToolrotoNew_4_15_Source_MultiUserTool_Navigator_h_38_EVENT_PARMS \
	struct Navigator_eventServerMoveForward_Parms \
	{ \
		float Value; \
		FRotator Rotation; \
	}; \
	struct Navigator_eventServerMoveRight_Parms \
	{ \
		float Value; \
		FRotator Rotation; \
	}; \
	struct Navigator_eventServerMoveUpDown_Parms \
	{ \
		float Value; \
		FRotator Rotation; \
	};


extern MULTIUSERTOOL_API  FName MULTIUSERTOOL_ServerMoveForward;
extern MULTIUSERTOOL_API  FName MULTIUSERTOOL_ServerMoveRight;
extern MULTIUSERTOOL_API  FName MULTIUSERTOOL_ServerMoveUpDown;
#define MultiUserToolrotoNew_4_15_Source_MultiUserTool_Navigator_h_38_CALLBACK_WRAPPERS
#define MultiUserToolrotoNew_4_15_Source_MultiUserTool_Navigator_h_38_INCLASS_NO_PURE_DECLS \
	private: \
	static void StaticRegisterNativesANavigator(); \
	friend MULTIUSERTOOL_API class UClass* Z_Construct_UClass_ANavigator(); \
	public: \
	DECLARE_CLASS(ANavigator, APawn, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/MultiUserTool"), NO_API) \
	DECLARE_SERIALIZER(ANavigator) \
	/** Indicates whether the class is compiled into the engine */ \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC}; \
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


#define MultiUserToolrotoNew_4_15_Source_MultiUserTool_Navigator_h_38_INCLASS \
	private: \
	static void StaticRegisterNativesANavigator(); \
	friend MULTIUSERTOOL_API class UClass* Z_Construct_UClass_ANavigator(); \
	public: \
	DECLARE_CLASS(ANavigator, APawn, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/MultiUserTool"), NO_API) \
	DECLARE_SERIALIZER(ANavigator) \
	/** Indicates whether the class is compiled into the engine */ \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC}; \
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


#define MultiUserToolrotoNew_4_15_Source_MultiUserTool_Navigator_h_38_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API ANavigator(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(ANavigator) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ANavigator); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ANavigator); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ANavigator(ANavigator&&); \
	NO_API ANavigator(const ANavigator&); \
public:


#define MultiUserToolrotoNew_4_15_Source_MultiUserTool_Navigator_h_38_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ANavigator(ANavigator&&); \
	NO_API ANavigator(const ANavigator&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ANavigator); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ANavigator); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(ANavigator)


#define MultiUserToolrotoNew_4_15_Source_MultiUserTool_Navigator_h_38_PRIVATE_PROPERTY_OFFSET \
	FORCEINLINE static uint32 __PPO__NavigatorMesh() { return STRUCT_OFFSET(ANavigator, NavigatorMesh); } \
	FORCEINLINE static uint32 __PPO__OurCameraSpringArm() { return STRUCT_OFFSET(ANavigator, OurCameraSpringArm); } \
	FORCEINLINE static uint32 __PPO__OurCamera() { return STRUCT_OFFSET(ANavigator, OurCamera); } \
	FORCEINLINE static uint32 __PPO__OurLightSpringArm() { return STRUCT_OFFSET(ANavigator, OurLightSpringArm); } \
	FORCEINLINE static uint32 __PPO__OurLight2() { return STRUCT_OFFSET(ANavigator, OurLight2); } \
	FORCEINLINE static uint32 __PPO__DirLight() { return STRUCT_OFFSET(ANavigator, DirLight); }


#define MultiUserToolrotoNew_4_15_Source_MultiUserTool_Navigator_h_35_PROLOG \
	MultiUserToolrotoNew_4_15_Source_MultiUserTool_Navigator_h_38_EVENT_PARMS


#define MultiUserToolrotoNew_4_15_Source_MultiUserTool_Navigator_h_38_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	MultiUserToolrotoNew_4_15_Source_MultiUserTool_Navigator_h_38_PRIVATE_PROPERTY_OFFSET \
	MultiUserToolrotoNew_4_15_Source_MultiUserTool_Navigator_h_38_RPC_WRAPPERS \
	MultiUserToolrotoNew_4_15_Source_MultiUserTool_Navigator_h_38_CALLBACK_WRAPPERS \
	MultiUserToolrotoNew_4_15_Source_MultiUserTool_Navigator_h_38_INCLASS \
	MultiUserToolrotoNew_4_15_Source_MultiUserTool_Navigator_h_38_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define MultiUserToolrotoNew_4_15_Source_MultiUserTool_Navigator_h_38_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	MultiUserToolrotoNew_4_15_Source_MultiUserTool_Navigator_h_38_PRIVATE_PROPERTY_OFFSET \
	MultiUserToolrotoNew_4_15_Source_MultiUserTool_Navigator_h_38_RPC_WRAPPERS_NO_PURE_DECLS \
	MultiUserToolrotoNew_4_15_Source_MultiUserTool_Navigator_h_38_CALLBACK_WRAPPERS \
	MultiUserToolrotoNew_4_15_Source_MultiUserTool_Navigator_h_38_INCLASS_NO_PURE_DECLS \
	MultiUserToolrotoNew_4_15_Source_MultiUserTool_Navigator_h_38_ENHANCED_CONSTRUCTORS \
static_assert(false, "Unknown access specifier for GENERATED_BODY() macro in class Navigator."); \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID MultiUserToolrotoNew_4_15_Source_MultiUserTool_Navigator_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
