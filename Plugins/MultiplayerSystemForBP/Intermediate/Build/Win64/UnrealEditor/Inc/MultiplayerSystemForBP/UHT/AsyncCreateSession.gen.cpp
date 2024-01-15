// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "MultiplayerSystemForBP/SessionAction/AsyncCreateSession.h"
#include "MultiplayerSystemForBP/Public/BlueprintDataDefinition.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeAsyncCreateSession() {}
// Cross Module References
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_APlayerController_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UBlueprintAsyncActionBase();
	MULTIPLAYERSYSTEMFORBP_API UClass* Z_Construct_UClass_UAsyncCreateSession();
	MULTIPLAYERSYSTEMFORBP_API UClass* Z_Construct_UClass_UAsyncCreateSession_NoRegister();
	MULTIPLAYERSYSTEMFORBP_API UFunction* Z_Construct_UDelegateFunction_MultiplayerSystemForBP_AsyncResultDelegate__DelegateSignature();
	MULTIPLAYERSYSTEMFORBP_API UScriptStruct* Z_Construct_UScriptStruct_FSessionFlagSettings();
	UPackage* Z_Construct_UPackage__Script_MultiplayerSystemForBP();
// End Cross Module References
	DEFINE_FUNCTION(UAsyncCreateSession::execAsyncCreateSession)
	{
		P_GET_OBJECT(UObject,Z_Param_WorldContextObject);
		P_GET_STRUCT(FSessionFlagSettings,Z_Param_SearchFlagSetting);
		P_GET_OBJECT(APlayerController,Z_Param_PlayerController);
		P_GET_PROPERTY(FIntProperty,Z_Param_PublicConnections);
		P_GET_PROPERTY(FIntProperty,Z_Param_PrivateConnections);
		P_GET_UBOOL(Z_Param_bUseLAN);
		P_GET_UBOOL(Z_Param_bAllowInvites);
		P_GET_UBOOL(Z_Param_bIsDedicatedServer);
		P_GET_UBOOL(Z_Param_bUsePresence);
		P_GET_UBOOL(Z_Param_bUseLobbiesIfAvailable);
		P_GET_UBOOL(Z_Param_bAllowJoinViaPresence);
		P_GET_UBOOL(Z_Param_bAllowJoinViaPresenceFriendsOnly);
		P_GET_UBOOL(Z_Param_bAntiCheatProtected);
		P_GET_UBOOL(Z_Param_bUsesStats);
		P_GET_UBOOL(Z_Param_bShouldAdvertise);
		P_GET_UBOOL(Z_Param_bUseLobbiesVoiceChatIfAvailable);
		P_GET_UBOOL(Z_Param_bStartAfterCreate);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(UAsyncCreateSession**)Z_Param__Result=UAsyncCreateSession::AsyncCreateSession(Z_Param_WorldContextObject,Z_Param_SearchFlagSetting,Z_Param_PlayerController,Z_Param_PublicConnections,Z_Param_PrivateConnections,Z_Param_bUseLAN,Z_Param_bAllowInvites,Z_Param_bIsDedicatedServer,Z_Param_bUsePresence,Z_Param_bUseLobbiesIfAvailable,Z_Param_bAllowJoinViaPresence,Z_Param_bAllowJoinViaPresenceFriendsOnly,Z_Param_bAntiCheatProtected,Z_Param_bUsesStats,Z_Param_bShouldAdvertise,Z_Param_bUseLobbiesVoiceChatIfAvailable,Z_Param_bStartAfterCreate);
		P_NATIVE_END;
	}
	void UAsyncCreateSession::StaticRegisterNativesUAsyncCreateSession()
	{
		UClass* Class = UAsyncCreateSession::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "AsyncCreateSession", &UAsyncCreateSession::execAsyncCreateSession },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics
	{
		struct AsyncCreateSession_eventAsyncCreateSession_Parms
		{
			UObject* WorldContextObject;
			FSessionFlagSettings SearchFlagSetting;
			APlayerController* PlayerController;
			int32 PublicConnections;
			int32 PrivateConnections;
			bool bUseLAN;
			bool bAllowInvites;
			bool bIsDedicatedServer;
			bool bUsePresence;
			bool bUseLobbiesIfAvailable;
			bool bAllowJoinViaPresence;
			bool bAllowJoinViaPresenceFriendsOnly;
			bool bAntiCheatProtected;
			bool bUsesStats;
			bool bShouldAdvertise;
			bool bUseLobbiesVoiceChatIfAvailable;
			bool bStartAfterCreate;
			UAsyncCreateSession* ReturnValue;
		};
		static const UECodeGen_Private::FObjectPropertyParams NewProp_WorldContextObject;
		static const UECodeGen_Private::FStructPropertyParams NewProp_SearchFlagSetting;
		static const UECodeGen_Private::FObjectPropertyParams NewProp_PlayerController;
		static const UECodeGen_Private::FIntPropertyParams NewProp_PublicConnections;
		static const UECodeGen_Private::FIntPropertyParams NewProp_PrivateConnections;
		static void NewProp_bUseLAN_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bUseLAN;
		static void NewProp_bAllowInvites_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bAllowInvites;
		static void NewProp_bIsDedicatedServer_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bIsDedicatedServer;
		static void NewProp_bUsePresence_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bUsePresence;
		static void NewProp_bUseLobbiesIfAvailable_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bUseLobbiesIfAvailable;
		static void NewProp_bAllowJoinViaPresence_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bAllowJoinViaPresence;
		static void NewProp_bAllowJoinViaPresenceFriendsOnly_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bAllowJoinViaPresenceFriendsOnly;
		static void NewProp_bAntiCheatProtected_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bAntiCheatProtected;
		static void NewProp_bUsesStats_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bUsesStats;
		static void NewProp_bShouldAdvertise_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bShouldAdvertise;
		static void NewProp_bUseLobbiesVoiceChatIfAvailable_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bUseLobbiesVoiceChatIfAvailable;
		static void NewProp_bStartAfterCreate_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bStartAfterCreate;
		static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_WorldContextObject = { "WorldContextObject", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(AsyncCreateSession_eventAsyncCreateSession_Parms, WorldContextObject), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_SearchFlagSetting = { "SearchFlagSetting", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(AsyncCreateSession_eventAsyncCreateSession_Parms, SearchFlagSetting), Z_Construct_UScriptStruct_FSessionFlagSettings, METADATA_PARAMS(nullptr, 0) }; // 4130689200
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_PlayerController = { "PlayerController", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(AsyncCreateSession_eventAsyncCreateSession_Parms, PlayerController), Z_Construct_UClass_APlayerController_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_PublicConnections = { "PublicConnections", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(AsyncCreateSession_eventAsyncCreateSession_Parms, PublicConnections), METADATA_PARAMS(nullptr, 0) };
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_PrivateConnections = { "PrivateConnections", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(AsyncCreateSession_eventAsyncCreateSession_Parms, PrivateConnections), METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bUseLAN_SetBit(void* Obj)
	{
		((AsyncCreateSession_eventAsyncCreateSession_Parms*)Obj)->bUseLAN = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bUseLAN = { "bUseLAN", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, sizeof(bool), sizeof(AsyncCreateSession_eventAsyncCreateSession_Parms), &Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bUseLAN_SetBit, METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bAllowInvites_SetBit(void* Obj)
	{
		((AsyncCreateSession_eventAsyncCreateSession_Parms*)Obj)->bAllowInvites = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bAllowInvites = { "bAllowInvites", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, sizeof(bool), sizeof(AsyncCreateSession_eventAsyncCreateSession_Parms), &Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bAllowInvites_SetBit, METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bIsDedicatedServer_SetBit(void* Obj)
	{
		((AsyncCreateSession_eventAsyncCreateSession_Parms*)Obj)->bIsDedicatedServer = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bIsDedicatedServer = { "bIsDedicatedServer", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, sizeof(bool), sizeof(AsyncCreateSession_eventAsyncCreateSession_Parms), &Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bIsDedicatedServer_SetBit, METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bUsePresence_SetBit(void* Obj)
	{
		((AsyncCreateSession_eventAsyncCreateSession_Parms*)Obj)->bUsePresence = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bUsePresence = { "bUsePresence", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, sizeof(bool), sizeof(AsyncCreateSession_eventAsyncCreateSession_Parms), &Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bUsePresence_SetBit, METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bUseLobbiesIfAvailable_SetBit(void* Obj)
	{
		((AsyncCreateSession_eventAsyncCreateSession_Parms*)Obj)->bUseLobbiesIfAvailable = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bUseLobbiesIfAvailable = { "bUseLobbiesIfAvailable", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, sizeof(bool), sizeof(AsyncCreateSession_eventAsyncCreateSession_Parms), &Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bUseLobbiesIfAvailable_SetBit, METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bAllowJoinViaPresence_SetBit(void* Obj)
	{
		((AsyncCreateSession_eventAsyncCreateSession_Parms*)Obj)->bAllowJoinViaPresence = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bAllowJoinViaPresence = { "bAllowJoinViaPresence", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, sizeof(bool), sizeof(AsyncCreateSession_eventAsyncCreateSession_Parms), &Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bAllowJoinViaPresence_SetBit, METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bAllowJoinViaPresenceFriendsOnly_SetBit(void* Obj)
	{
		((AsyncCreateSession_eventAsyncCreateSession_Parms*)Obj)->bAllowJoinViaPresenceFriendsOnly = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bAllowJoinViaPresenceFriendsOnly = { "bAllowJoinViaPresenceFriendsOnly", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, sizeof(bool), sizeof(AsyncCreateSession_eventAsyncCreateSession_Parms), &Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bAllowJoinViaPresenceFriendsOnly_SetBit, METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bAntiCheatProtected_SetBit(void* Obj)
	{
		((AsyncCreateSession_eventAsyncCreateSession_Parms*)Obj)->bAntiCheatProtected = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bAntiCheatProtected = { "bAntiCheatProtected", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, sizeof(bool), sizeof(AsyncCreateSession_eventAsyncCreateSession_Parms), &Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bAntiCheatProtected_SetBit, METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bUsesStats_SetBit(void* Obj)
	{
		((AsyncCreateSession_eventAsyncCreateSession_Parms*)Obj)->bUsesStats = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bUsesStats = { "bUsesStats", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, sizeof(bool), sizeof(AsyncCreateSession_eventAsyncCreateSession_Parms), &Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bUsesStats_SetBit, METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bShouldAdvertise_SetBit(void* Obj)
	{
		((AsyncCreateSession_eventAsyncCreateSession_Parms*)Obj)->bShouldAdvertise = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bShouldAdvertise = { "bShouldAdvertise", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, sizeof(bool), sizeof(AsyncCreateSession_eventAsyncCreateSession_Parms), &Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bShouldAdvertise_SetBit, METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bUseLobbiesVoiceChatIfAvailable_SetBit(void* Obj)
	{
		((AsyncCreateSession_eventAsyncCreateSession_Parms*)Obj)->bUseLobbiesVoiceChatIfAvailable = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bUseLobbiesVoiceChatIfAvailable = { "bUseLobbiesVoiceChatIfAvailable", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, sizeof(bool), sizeof(AsyncCreateSession_eventAsyncCreateSession_Parms), &Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bUseLobbiesVoiceChatIfAvailable_SetBit, METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bStartAfterCreate_SetBit(void* Obj)
	{
		((AsyncCreateSession_eventAsyncCreateSession_Parms*)Obj)->bStartAfterCreate = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bStartAfterCreate = { "bStartAfterCreate", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, sizeof(bool), sizeof(AsyncCreateSession_eventAsyncCreateSession_Parms), &Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bStartAfterCreate_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(AsyncCreateSession_eventAsyncCreateSession_Parms, ReturnValue), Z_Construct_UClass_UAsyncCreateSession_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_WorldContextObject,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_SearchFlagSetting,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_PlayerController,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_PublicConnections,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_PrivateConnections,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bUseLAN,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bAllowInvites,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bIsDedicatedServer,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bUsePresence,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bUseLobbiesIfAvailable,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bAllowJoinViaPresence,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bAllowJoinViaPresenceFriendsOnly,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bAntiCheatProtected,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bUsesStats,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bShouldAdvertise,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bUseLobbiesVoiceChatIfAvailable,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_bStartAfterCreate,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::Function_MetaDataParams[] = {
		{ "BlueprintInternalUseOnly", "true" },
		{ "Category", "Online|AsnycSessions" },
		{ "Comment", "//const TArray<FSessionPropertyKeyPair>& ExtraSettings,\n" },
		{ "CPP_Default_bAllowInvites", "true" },
		{ "CPP_Default_bAllowJoinViaPresence", "true" },
		{ "CPP_Default_bAllowJoinViaPresenceFriendsOnly", "false" },
		{ "CPP_Default_bAntiCheatProtected", "false" },
		{ "CPP_Default_bIsDedicatedServer", "false" },
		{ "CPP_Default_bShouldAdvertise", "true" },
		{ "CPP_Default_bStartAfterCreate", "true" },
		{ "CPP_Default_bUseLAN", "false" },
		{ "CPP_Default_bUseLobbiesIfAvailable", "true" },
		{ "CPP_Default_bUseLobbiesVoiceChatIfAvailable", "false" },
		{ "CPP_Default_bUsePresence", "true" },
		{ "CPP_Default_bUsesStats", "false" },
		{ "CPP_Default_PlayerController", "None" },
		{ "CPP_Default_PrivateConnections", "0" },
		{ "CPP_Default_PublicConnections", "2" },
		{ "ModuleRelativePath", "SessionAction/AsyncCreateSession.h" },
		{ "ToolTip", "const TArray<FSessionPropertyKeyPair>& ExtraSettings," },
		{ "WorldContext", "WorldContextObject" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UAsyncCreateSession, nullptr, "AsyncCreateSession", nullptr, nullptr, sizeof(Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::AsyncCreateSession_eventAsyncCreateSession_Parms), Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UAsyncCreateSession);
	UClass* Z_Construct_UClass_UAsyncCreateSession_NoRegister()
	{
		return UAsyncCreateSession::StaticClass();
	}
	struct Z_Construct_UClass_UAsyncCreateSession_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_OnSuccess_MetaData[];
#endif
		static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnSuccess;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_OnFailure_MetaData[];
#endif
		static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnFailure;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UAsyncCreateSession_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UBlueprintAsyncActionBase,
		(UObject* (*)())Z_Construct_UPackage__Script_MultiplayerSystemForBP,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_UAsyncCreateSession_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UAsyncCreateSession_AsyncCreateSession, "AsyncCreateSession" }, // 707365052
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UAsyncCreateSession_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "IncludePath", "SessionAction/AsyncCreateSession.h" },
		{ "ModuleRelativePath", "SessionAction/AsyncCreateSession.h" },
		{ "ObjectInitializerConstructorDeclared", "" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UAsyncCreateSession_Statics::NewProp_OnSuccess_MetaData[] = {
		{ "ModuleRelativePath", "SessionAction/AsyncCreateSession.h" },
	};
#endif
	const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UAsyncCreateSession_Statics::NewProp_OnSuccess = { "OnSuccess", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UAsyncCreateSession, OnSuccess), Z_Construct_UDelegateFunction_MultiplayerSystemForBP_AsyncResultDelegate__DelegateSignature, METADATA_PARAMS(Z_Construct_UClass_UAsyncCreateSession_Statics::NewProp_OnSuccess_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UAsyncCreateSession_Statics::NewProp_OnSuccess_MetaData)) }; // 838674140
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UAsyncCreateSession_Statics::NewProp_OnFailure_MetaData[] = {
		{ "ModuleRelativePath", "SessionAction/AsyncCreateSession.h" },
	};
#endif
	const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UAsyncCreateSession_Statics::NewProp_OnFailure = { "OnFailure", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UAsyncCreateSession, OnFailure), Z_Construct_UDelegateFunction_MultiplayerSystemForBP_AsyncResultDelegate__DelegateSignature, METADATA_PARAMS(Z_Construct_UClass_UAsyncCreateSession_Statics::NewProp_OnFailure_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UAsyncCreateSession_Statics::NewProp_OnFailure_MetaData)) }; // 838674140
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UAsyncCreateSession_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UAsyncCreateSession_Statics::NewProp_OnSuccess,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UAsyncCreateSession_Statics::NewProp_OnFailure,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UAsyncCreateSession_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UAsyncCreateSession>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UAsyncCreateSession_Statics::ClassParams = {
		&UAsyncCreateSession::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		Z_Construct_UClass_UAsyncCreateSession_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		UE_ARRAY_COUNT(Z_Construct_UClass_UAsyncCreateSession_Statics::PropPointers),
		0,
		0x009000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UAsyncCreateSession_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UAsyncCreateSession_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UAsyncCreateSession()
	{
		if (!Z_Registration_Info_UClass_UAsyncCreateSession.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UAsyncCreateSession.OuterSingleton, Z_Construct_UClass_UAsyncCreateSession_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UAsyncCreateSession.OuterSingleton;
	}
	template<> MULTIPLAYERSYSTEMFORBP_API UClass* StaticClass<UAsyncCreateSession>()
	{
		return UAsyncCreateSession::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UAsyncCreateSession);
	UAsyncCreateSession::~UAsyncCreateSession() {}
	struct Z_CompiledInDeferFile_FID_gamecreating_PROJECT_MultiPlayerSystem_Plugins_MultiplayerSystemForBP_Source_MultiplayerSystemForBP_SessionAction_AsyncCreateSession_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_gamecreating_PROJECT_MultiPlayerSystem_Plugins_MultiplayerSystemForBP_Source_MultiplayerSystemForBP_SessionAction_AsyncCreateSession_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UAsyncCreateSession, UAsyncCreateSession::StaticClass, TEXT("UAsyncCreateSession"), &Z_Registration_Info_UClass_UAsyncCreateSession, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UAsyncCreateSession), 2744365944U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_gamecreating_PROJECT_MultiPlayerSystem_Plugins_MultiplayerSystemForBP_Source_MultiplayerSystemForBP_SessionAction_AsyncCreateSession_h_1662412868(TEXT("/Script/MultiplayerSystemForBP"),
		Z_CompiledInDeferFile_FID_gamecreating_PROJECT_MultiPlayerSystem_Plugins_MultiplayerSystemForBP_Source_MultiplayerSystemForBP_SessionAction_AsyncCreateSession_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_gamecreating_PROJECT_MultiPlayerSystem_Plugins_MultiplayerSystemForBP_Source_MultiplayerSystemForBP_SessionAction_AsyncCreateSession_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
