// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "BlueprintDataDefinition.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef MULTIPLAYERSYSTEMFORBP_BlueprintDataDefinition_generated_h
#error "BlueprintDataDefinition.generated.h already included, missing '#pragma once' in BlueprintDataDefinition.h"
#endif
#define MULTIPLAYERSYSTEMFORBP_BlueprintDataDefinition_generated_h

#define FID_gamecreating_PROJECT_MultiPlayerSystem_Plugins_MultiplayerSystemForBP_Source_MultiplayerSystemForBP_Public_BlueprintDataDefinition_h_6_DELEGATE \
MULTIPLAYERSYSTEMFORBP_API void FAsyncResultDelegate_DelegateWrapper(const FMulticastScriptDelegate& AsyncResultDelegate);


#define FID_gamecreating_PROJECT_MultiPlayerSystem_Plugins_MultiplayerSystemForBP_Source_MultiplayerSystemForBP_Public_BlueprintDataDefinition_h_11_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FSessionPropertyKeyPair_Statics; \
	MULTIPLAYERSYSTEMFORBP_API static class UScriptStruct* StaticStruct();


template<> MULTIPLAYERSYSTEMFORBP_API UScriptStruct* StaticStruct<struct FSessionPropertyKeyPair>();

#define FID_gamecreating_PROJECT_MultiPlayerSystem_Plugins_MultiplayerSystemForBP_Source_MultiplayerSystemForBP_Public_BlueprintDataDefinition_h_21_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FSessionFlagForBool_Statics; \
	MULTIPLAYERSYSTEMFORBP_API static class UScriptStruct* StaticStruct();


template<> MULTIPLAYERSYSTEMFORBP_API UScriptStruct* StaticStruct<struct FSessionFlagForBool>();

#define FID_gamecreating_PROJECT_MultiPlayerSystem_Plugins_MultiplayerSystemForBP_Source_MultiplayerSystemForBP_Public_BlueprintDataDefinition_h_31_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FSessionFlagForInt_Statics; \
	MULTIPLAYERSYSTEMFORBP_API static class UScriptStruct* StaticStruct();


template<> MULTIPLAYERSYSTEMFORBP_API UScriptStruct* StaticStruct<struct FSessionFlagForInt>();

#define FID_gamecreating_PROJECT_MultiPlayerSystem_Plugins_MultiplayerSystemForBP_Source_MultiplayerSystemForBP_Public_BlueprintDataDefinition_h_41_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FSessionFlagForString_Statics; \
	MULTIPLAYERSYSTEMFORBP_API static class UScriptStruct* StaticStruct();


template<> MULTIPLAYERSYSTEMFORBP_API UScriptStruct* StaticStruct<struct FSessionFlagForString>();

#define FID_gamecreating_PROJECT_MultiPlayerSystem_Plugins_MultiplayerSystemForBP_Source_MultiplayerSystemForBP_Public_BlueprintDataDefinition_h_51_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FSessionFlagSettings_Statics; \
	MULTIPLAYERSYSTEMFORBP_API static class UScriptStruct* StaticStruct();


template<> MULTIPLAYERSYSTEMFORBP_API UScriptStruct* StaticStruct<struct FSessionFlagSettings>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_gamecreating_PROJECT_MultiPlayerSystem_Plugins_MultiplayerSystemForBP_Source_MultiplayerSystemForBP_Public_BlueprintDataDefinition_h


#define FOREACH_ENUM_ESESSIONSTATE(op) \
	op(ESessionState::NoSession) \
	op(ESessionState::Creating) \
	op(ESessionState::Pending) \
	op(ESessionState::Starting) \
	op(ESessionState::InProgress) \
	op(ESessionState::Ending) \
	op(ESessionState::Ended) \
	op(ESessionState::Destroying) 

enum class ESessionState;
template<> struct TIsUEnumClass<ESessionState> { enum { Value = true }; };
template<> MULTIPLAYERSYSTEMFORBP_API UEnum* StaticEnum<ESessionState>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
