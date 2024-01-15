// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "MultiplayerSystemForBP/Public/BlueprintDataDefinition.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBlueprintDataDefinition() {}
// Cross Module References
	MULTIPLAYERSYSTEMFORBP_API UEnum* Z_Construct_UEnum_MultiplayerSystemForBP_ESessionState();
	MULTIPLAYERSYSTEMFORBP_API UFunction* Z_Construct_UDelegateFunction_MultiplayerSystemForBP_AsyncResultDelegate__DelegateSignature();
	MULTIPLAYERSYSTEMFORBP_API UScriptStruct* Z_Construct_UScriptStruct_FSessionFlagForBool();
	MULTIPLAYERSYSTEMFORBP_API UScriptStruct* Z_Construct_UScriptStruct_FSessionFlagForInt();
	MULTIPLAYERSYSTEMFORBP_API UScriptStruct* Z_Construct_UScriptStruct_FSessionFlagForString();
	MULTIPLAYERSYSTEMFORBP_API UScriptStruct* Z_Construct_UScriptStruct_FSessionFlagSettings();
	MULTIPLAYERSYSTEMFORBP_API UScriptStruct* Z_Construct_UScriptStruct_FSessionPropertyKeyPair();
	UPackage* Z_Construct_UPackage__Script_MultiplayerSystemForBP();
// End Cross Module References
	struct Z_Construct_UDelegateFunction_MultiplayerSystemForBP_AsyncResultDelegate__DelegateSignature_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UDelegateFunction_MultiplayerSystemForBP_AsyncResultDelegate__DelegateSignature_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/BlueprintDataDefinition.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UDelegateFunction_MultiplayerSystemForBP_AsyncResultDelegate__DelegateSignature_Statics::FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_MultiplayerSystemForBP, nullptr, "AsyncResultDelegate__DelegateSignature", nullptr, nullptr, 0, nullptr, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(Z_Construct_UDelegateFunction_MultiplayerSystemForBP_AsyncResultDelegate__DelegateSignature_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_MultiplayerSystemForBP_AsyncResultDelegate__DelegateSignature_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UDelegateFunction_MultiplayerSystemForBP_AsyncResultDelegate__DelegateSignature()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UDelegateFunction_MultiplayerSystemForBP_AsyncResultDelegate__DelegateSignature_Statics::FuncParams);
		}
		return ReturnFunction;
	}
void FAsyncResultDelegate_DelegateWrapper(const FMulticastScriptDelegate& AsyncResultDelegate)
{
	AsyncResultDelegate.ProcessMulticastDelegate<UObject>(NULL);
}
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_SessionPropertyKeyPair;
class UScriptStruct* FSessionPropertyKeyPair::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_SessionPropertyKeyPair.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_SessionPropertyKeyPair.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FSessionPropertyKeyPair, (UObject*)Z_Construct_UPackage__Script_MultiplayerSystemForBP(), TEXT("SessionPropertyKeyPair"));
	}
	return Z_Registration_Info_UScriptStruct_SessionPropertyKeyPair.OuterSingleton;
}
template<> MULTIPLAYERSYSTEMFORBP_API UScriptStruct* StaticStruct<FSessionPropertyKeyPair>()
{
	return FSessionPropertyKeyPair::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FSessionPropertyKeyPair_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FSessionPropertyKeyPair_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/BlueprintDataDefinition.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FSessionPropertyKeyPair_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FSessionPropertyKeyPair>();
	}
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FSessionPropertyKeyPair_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_MultiplayerSystemForBP,
		nullptr,
		&NewStructOps,
		"SessionPropertyKeyPair",
		sizeof(FSessionPropertyKeyPair),
		alignof(FSessionPropertyKeyPair),
		nullptr,
		0,
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FSessionPropertyKeyPair_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSessionPropertyKeyPair_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FSessionPropertyKeyPair()
	{
		if (!Z_Registration_Info_UScriptStruct_SessionPropertyKeyPair.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_SessionPropertyKeyPair.InnerSingleton, Z_Construct_UScriptStruct_FSessionPropertyKeyPair_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_SessionPropertyKeyPair.InnerSingleton;
	}
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_SessionFlagForBool;
class UScriptStruct* FSessionFlagForBool::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_SessionFlagForBool.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_SessionFlagForBool.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FSessionFlagForBool, (UObject*)Z_Construct_UPackage__Script_MultiplayerSystemForBP(), TEXT("SessionFlagForBool"));
	}
	return Z_Registration_Info_UScriptStruct_SessionFlagForBool.OuterSingleton;
}
template<> MULTIPLAYERSYSTEMFORBP_API UScriptStruct* StaticStruct<FSessionFlagForBool>()
{
	return FSessionFlagForBool::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FSessionFlagForBool_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Key_MetaData[];
#endif
		static const UECodeGen_Private::FNamePropertyParams NewProp_Key;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Value_MetaData[];
#endif
		static void NewProp_Value_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_Value;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FSessionFlagForBool_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/BlueprintDataDefinition.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FSessionFlagForBool_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FSessionFlagForBool>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FSessionFlagForBool_Statics::NewProp_Key_MetaData[] = {
		{ "Category", "SessionFlagForBool" },
		{ "ModuleRelativePath", "Public/BlueprintDataDefinition.h" },
	};
#endif
	const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FSessionFlagForBool_Statics::NewProp_Key = { "Key", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(FSessionFlagForBool, Key), METADATA_PARAMS(Z_Construct_UScriptStruct_FSessionFlagForBool_Statics::NewProp_Key_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSessionFlagForBool_Statics::NewProp_Key_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FSessionFlagForBool_Statics::NewProp_Value_MetaData[] = {
		{ "Category", "SessionFlagForBool" },
		{ "ModuleRelativePath", "Public/BlueprintDataDefinition.h" },
	};
#endif
	void Z_Construct_UScriptStruct_FSessionFlagForBool_Statics::NewProp_Value_SetBit(void* Obj)
	{
		((FSessionFlagForBool*)Obj)->Value = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FSessionFlagForBool_Statics::NewProp_Value = { "Value", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, sizeof(bool), sizeof(FSessionFlagForBool), &Z_Construct_UScriptStruct_FSessionFlagForBool_Statics::NewProp_Value_SetBit, METADATA_PARAMS(Z_Construct_UScriptStruct_FSessionFlagForBool_Statics::NewProp_Value_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSessionFlagForBool_Statics::NewProp_Value_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FSessionFlagForBool_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FSessionFlagForBool_Statics::NewProp_Key,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FSessionFlagForBool_Statics::NewProp_Value,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FSessionFlagForBool_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_MultiplayerSystemForBP,
		nullptr,
		&NewStructOps,
		"SessionFlagForBool",
		sizeof(FSessionFlagForBool),
		alignof(FSessionFlagForBool),
		Z_Construct_UScriptStruct_FSessionFlagForBool_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSessionFlagForBool_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FSessionFlagForBool_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSessionFlagForBool_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FSessionFlagForBool()
	{
		if (!Z_Registration_Info_UScriptStruct_SessionFlagForBool.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_SessionFlagForBool.InnerSingleton, Z_Construct_UScriptStruct_FSessionFlagForBool_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_SessionFlagForBool.InnerSingleton;
	}
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_SessionFlagForInt;
class UScriptStruct* FSessionFlagForInt::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_SessionFlagForInt.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_SessionFlagForInt.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FSessionFlagForInt, (UObject*)Z_Construct_UPackage__Script_MultiplayerSystemForBP(), TEXT("SessionFlagForInt"));
	}
	return Z_Registration_Info_UScriptStruct_SessionFlagForInt.OuterSingleton;
}
template<> MULTIPLAYERSYSTEMFORBP_API UScriptStruct* StaticStruct<FSessionFlagForInt>()
{
	return FSessionFlagForInt::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FSessionFlagForInt_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Key_MetaData[];
#endif
		static const UECodeGen_Private::FNamePropertyParams NewProp_Key;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Value_MetaData[];
#endif
		static const UECodeGen_Private::FUnsizedIntPropertyParams NewProp_Value;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FSessionFlagForInt_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/BlueprintDataDefinition.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FSessionFlagForInt_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FSessionFlagForInt>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FSessionFlagForInt_Statics::NewProp_Key_MetaData[] = {
		{ "Category", "SessionFlagForInt" },
		{ "ModuleRelativePath", "Public/BlueprintDataDefinition.h" },
	};
#endif
	const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FSessionFlagForInt_Statics::NewProp_Key = { "Key", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(FSessionFlagForInt, Key), METADATA_PARAMS(Z_Construct_UScriptStruct_FSessionFlagForInt_Statics::NewProp_Key_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSessionFlagForInt_Statics::NewProp_Key_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FSessionFlagForInt_Statics::NewProp_Value_MetaData[] = {
		{ "Category", "SessionFlagForInt" },
		{ "ModuleRelativePath", "Public/BlueprintDataDefinition.h" },
	};
#endif
	const UECodeGen_Private::FUnsizedIntPropertyParams Z_Construct_UScriptStruct_FSessionFlagForInt_Statics::NewProp_Value = { "Value", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(FSessionFlagForInt, Value), METADATA_PARAMS(Z_Construct_UScriptStruct_FSessionFlagForInt_Statics::NewProp_Value_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSessionFlagForInt_Statics::NewProp_Value_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FSessionFlagForInt_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FSessionFlagForInt_Statics::NewProp_Key,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FSessionFlagForInt_Statics::NewProp_Value,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FSessionFlagForInt_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_MultiplayerSystemForBP,
		nullptr,
		&NewStructOps,
		"SessionFlagForInt",
		sizeof(FSessionFlagForInt),
		alignof(FSessionFlagForInt),
		Z_Construct_UScriptStruct_FSessionFlagForInt_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSessionFlagForInt_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FSessionFlagForInt_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSessionFlagForInt_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FSessionFlagForInt()
	{
		if (!Z_Registration_Info_UScriptStruct_SessionFlagForInt.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_SessionFlagForInt.InnerSingleton, Z_Construct_UScriptStruct_FSessionFlagForInt_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_SessionFlagForInt.InnerSingleton;
	}
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_SessionFlagForString;
class UScriptStruct* FSessionFlagForString::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_SessionFlagForString.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_SessionFlagForString.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FSessionFlagForString, (UObject*)Z_Construct_UPackage__Script_MultiplayerSystemForBP(), TEXT("SessionFlagForString"));
	}
	return Z_Registration_Info_UScriptStruct_SessionFlagForString.OuterSingleton;
}
template<> MULTIPLAYERSYSTEMFORBP_API UScriptStruct* StaticStruct<FSessionFlagForString>()
{
	return FSessionFlagForString::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FSessionFlagForString_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Key_MetaData[];
#endif
		static const UECodeGen_Private::FNamePropertyParams NewProp_Key;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Value_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_Value;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FSessionFlagForString_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/BlueprintDataDefinition.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FSessionFlagForString_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FSessionFlagForString>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FSessionFlagForString_Statics::NewProp_Key_MetaData[] = {
		{ "Category", "SessionFlagForString" },
		{ "ModuleRelativePath", "Public/BlueprintDataDefinition.h" },
	};
#endif
	const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FSessionFlagForString_Statics::NewProp_Key = { "Key", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(FSessionFlagForString, Key), METADATA_PARAMS(Z_Construct_UScriptStruct_FSessionFlagForString_Statics::NewProp_Key_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSessionFlagForString_Statics::NewProp_Key_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FSessionFlagForString_Statics::NewProp_Value_MetaData[] = {
		{ "Category", "SessionFlagForString" },
		{ "ModuleRelativePath", "Public/BlueprintDataDefinition.h" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FSessionFlagForString_Statics::NewProp_Value = { "Value", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(FSessionFlagForString, Value), METADATA_PARAMS(Z_Construct_UScriptStruct_FSessionFlagForString_Statics::NewProp_Value_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSessionFlagForString_Statics::NewProp_Value_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FSessionFlagForString_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FSessionFlagForString_Statics::NewProp_Key,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FSessionFlagForString_Statics::NewProp_Value,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FSessionFlagForString_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_MultiplayerSystemForBP,
		nullptr,
		&NewStructOps,
		"SessionFlagForString",
		sizeof(FSessionFlagForString),
		alignof(FSessionFlagForString),
		Z_Construct_UScriptStruct_FSessionFlagForString_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSessionFlagForString_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FSessionFlagForString_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSessionFlagForString_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FSessionFlagForString()
	{
		if (!Z_Registration_Info_UScriptStruct_SessionFlagForString.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_SessionFlagForString.InnerSingleton, Z_Construct_UScriptStruct_FSessionFlagForString_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_SessionFlagForString.InnerSingleton;
	}
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_SessionFlagSettings;
class UScriptStruct* FSessionFlagSettings::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_SessionFlagSettings.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_SessionFlagSettings.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FSessionFlagSettings, (UObject*)Z_Construct_UPackage__Script_MultiplayerSystemForBP(), TEXT("SessionFlagSettings"));
	}
	return Z_Registration_Info_UScriptStruct_SessionFlagSettings.OuterSingleton;
}
template<> MULTIPLAYERSYSTEMFORBP_API UScriptStruct* StaticStruct<FSessionFlagSettings>()
{
	return FSessionFlagSettings::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FSessionFlagSettings_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
		static const UECodeGen_Private::FStructPropertyParams NewProp_StringFlags_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_StringFlags_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_StringFlags;
		static const UECodeGen_Private::FStructPropertyParams NewProp_IntFlags_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_IntFlags_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_IntFlags;
		static const UECodeGen_Private::FStructPropertyParams NewProp_BoolFlags_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BoolFlags_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_BoolFlags;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FSessionFlagSettings_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/BlueprintDataDefinition.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FSessionFlagSettings_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FSessionFlagSettings>();
	}
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FSessionFlagSettings_Statics::NewProp_StringFlags_Inner = { "StringFlags", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, 0, Z_Construct_UScriptStruct_FSessionFlagForString, METADATA_PARAMS(nullptr, 0) }; // 608991145
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FSessionFlagSettings_Statics::NewProp_StringFlags_MetaData[] = {
		{ "Category", "SessionFlagSettings" },
		{ "ModuleRelativePath", "Public/BlueprintDataDefinition.h" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FSessionFlagSettings_Statics::NewProp_StringFlags = { "StringFlags", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(FSessionFlagSettings, StringFlags), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UScriptStruct_FSessionFlagSettings_Statics::NewProp_StringFlags_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSessionFlagSettings_Statics::NewProp_StringFlags_MetaData)) }; // 608991145
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FSessionFlagSettings_Statics::NewProp_IntFlags_Inner = { "IntFlags", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, 0, Z_Construct_UScriptStruct_FSessionFlagForInt, METADATA_PARAMS(nullptr, 0) }; // 4051478677
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FSessionFlagSettings_Statics::NewProp_IntFlags_MetaData[] = {
		{ "Category", "SessionFlagSettings" },
		{ "ModuleRelativePath", "Public/BlueprintDataDefinition.h" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FSessionFlagSettings_Statics::NewProp_IntFlags = { "IntFlags", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(FSessionFlagSettings, IntFlags), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UScriptStruct_FSessionFlagSettings_Statics::NewProp_IntFlags_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSessionFlagSettings_Statics::NewProp_IntFlags_MetaData)) }; // 4051478677
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FSessionFlagSettings_Statics::NewProp_BoolFlags_Inner = { "BoolFlags", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, 0, Z_Construct_UScriptStruct_FSessionFlagForBool, METADATA_PARAMS(nullptr, 0) }; // 3784327995
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FSessionFlagSettings_Statics::NewProp_BoolFlags_MetaData[] = {
		{ "Category", "SessionFlagSettings" },
		{ "ModuleRelativePath", "Public/BlueprintDataDefinition.h" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FSessionFlagSettings_Statics::NewProp_BoolFlags = { "BoolFlags", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(FSessionFlagSettings, BoolFlags), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UScriptStruct_FSessionFlagSettings_Statics::NewProp_BoolFlags_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSessionFlagSettings_Statics::NewProp_BoolFlags_MetaData)) }; // 3784327995
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FSessionFlagSettings_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FSessionFlagSettings_Statics::NewProp_StringFlags_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FSessionFlagSettings_Statics::NewProp_StringFlags,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FSessionFlagSettings_Statics::NewProp_IntFlags_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FSessionFlagSettings_Statics::NewProp_IntFlags,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FSessionFlagSettings_Statics::NewProp_BoolFlags_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FSessionFlagSettings_Statics::NewProp_BoolFlags,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FSessionFlagSettings_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_MultiplayerSystemForBP,
		nullptr,
		&NewStructOps,
		"SessionFlagSettings",
		sizeof(FSessionFlagSettings),
		alignof(FSessionFlagSettings),
		Z_Construct_UScriptStruct_FSessionFlagSettings_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSessionFlagSettings_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FSessionFlagSettings_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSessionFlagSettings_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FSessionFlagSettings()
	{
		if (!Z_Registration_Info_UScriptStruct_SessionFlagSettings.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_SessionFlagSettings.InnerSingleton, Z_Construct_UScriptStruct_FSessionFlagSettings_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_SessionFlagSettings.InnerSingleton;
	}
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_ESessionState;
	static UEnum* ESessionState_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_ESessionState.OuterSingleton)
		{
			Z_Registration_Info_UEnum_ESessionState.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_MultiplayerSystemForBP_ESessionState, (UObject*)Z_Construct_UPackage__Script_MultiplayerSystemForBP(), TEXT("ESessionState"));
		}
		return Z_Registration_Info_UEnum_ESessionState.OuterSingleton;
	}
	template<> MULTIPLAYERSYSTEMFORBP_API UEnum* StaticEnum<ESessionState>()
	{
		return ESessionState_StaticEnum();
	}
	struct Z_Construct_UEnum_MultiplayerSystemForBP_ESessionState_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_MultiplayerSystemForBP_ESessionState_Statics::Enumerators[] = {
		{ "ESessionState::NoSession", (int64)ESessionState::NoSession },
		{ "ESessionState::Creating", (int64)ESessionState::Creating },
		{ "ESessionState::Pending", (int64)ESessionState::Pending },
		{ "ESessionState::Starting", (int64)ESessionState::Starting },
		{ "ESessionState::InProgress", (int64)ESessionState::InProgress },
		{ "ESessionState::Ending", (int64)ESessionState::Ending },
		{ "ESessionState::Ended", (int64)ESessionState::Ended },
		{ "ESessionState::Destroying", (int64)ESessionState::Destroying },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_MultiplayerSystemForBP_ESessionState_Statics::Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "Creating.Comment", "/** An online session is in the process of being created */" },
		{ "Creating.Name", "ESessionState::Creating" },
		{ "Creating.ToolTip", "An online session is in the process of being created" },
		{ "Destroying.Comment", "/** The session is being destroyed */" },
		{ "Destroying.Name", "ESessionState::Destroying" },
		{ "Destroying.ToolTip", "The session is being destroyed" },
		{ "Ended.Comment", "/** The session is closed and any stats committed */" },
		{ "Ended.Name", "ESessionState::Ended" },
		{ "Ended.ToolTip", "The session is closed and any stats committed" },
		{ "Ending.Comment", "/** The session is still valid, but the session is no longer being played (post match lobby) */" },
		{ "Ending.Name", "ESessionState::Ending" },
		{ "Ending.ToolTip", "The session is still valid, but the session is no longer being played (post match lobby)" },
		{ "InProgress.Comment", "/** The current session has started. Sessions with join in progress disabled are no longer joinable */" },
		{ "InProgress.Name", "ESessionState::InProgress" },
		{ "InProgress.ToolTip", "The current session has started. Sessions with join in progress disabled are no longer joinable" },
		{ "ModuleRelativePath", "Public/BlueprintDataDefinition.h" },
		{ "NoSession.Comment", "/** An online session has not been created yet */" },
		{ "NoSession.Name", "ESessionState::NoSession" },
		{ "NoSession.ToolTip", "An online session has not been created yet" },
		{ "Pending.Comment", "/** Session has been created but the session hasn't started (pre match lobby) */" },
		{ "Pending.Name", "ESessionState::Pending" },
		{ "Pending.ToolTip", "Session has been created but the session hasn't started (pre match lobby)" },
		{ "Starting.Comment", "/** Session has been asked to start (may take time due to communication with backend) */" },
		{ "Starting.Name", "ESessionState::Starting" },
		{ "Starting.ToolTip", "Session has been asked to start (may take time due to communication with backend)" },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_MultiplayerSystemForBP_ESessionState_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_MultiplayerSystemForBP,
		nullptr,
		"ESessionState",
		"ESessionState",
		Z_Construct_UEnum_MultiplayerSystemForBP_ESessionState_Statics::Enumerators,
		UE_ARRAY_COUNT(Z_Construct_UEnum_MultiplayerSystemForBP_ESessionState_Statics::Enumerators),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(Z_Construct_UEnum_MultiplayerSystemForBP_ESessionState_Statics::Enum_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UEnum_MultiplayerSystemForBP_ESessionState_Statics::Enum_MetaDataParams))
	};
	UEnum* Z_Construct_UEnum_MultiplayerSystemForBP_ESessionState()
	{
		if (!Z_Registration_Info_UEnum_ESessionState.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_ESessionState.InnerSingleton, Z_Construct_UEnum_MultiplayerSystemForBP_ESessionState_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_ESessionState.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_gamecreating_PROJECT_MultiPlayerSystem_Plugins_MultiplayerSystemForBP_Source_MultiplayerSystemForBP_Public_BlueprintDataDefinition_h_Statics
	{
		static const FEnumRegisterCompiledInInfo EnumInfo[];
		static const FStructRegisterCompiledInInfo ScriptStructInfo[];
	};
	const FEnumRegisterCompiledInInfo Z_CompiledInDeferFile_FID_gamecreating_PROJECT_MultiPlayerSystem_Plugins_MultiplayerSystemForBP_Source_MultiplayerSystemForBP_Public_BlueprintDataDefinition_h_Statics::EnumInfo[] = {
		{ ESessionState_StaticEnum, TEXT("ESessionState"), &Z_Registration_Info_UEnum_ESessionState, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 4144908339U) },
	};
	const FStructRegisterCompiledInInfo Z_CompiledInDeferFile_FID_gamecreating_PROJECT_MultiPlayerSystem_Plugins_MultiplayerSystemForBP_Source_MultiplayerSystemForBP_Public_BlueprintDataDefinition_h_Statics::ScriptStructInfo[] = {
		{ FSessionPropertyKeyPair::StaticStruct, Z_Construct_UScriptStruct_FSessionPropertyKeyPair_Statics::NewStructOps, TEXT("SessionPropertyKeyPair"), &Z_Registration_Info_UScriptStruct_SessionPropertyKeyPair, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FSessionPropertyKeyPair), 2004437623U) },
		{ FSessionFlagForBool::StaticStruct, Z_Construct_UScriptStruct_FSessionFlagForBool_Statics::NewStructOps, TEXT("SessionFlagForBool"), &Z_Registration_Info_UScriptStruct_SessionFlagForBool, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FSessionFlagForBool), 3784327995U) },
		{ FSessionFlagForInt::StaticStruct, Z_Construct_UScriptStruct_FSessionFlagForInt_Statics::NewStructOps, TEXT("SessionFlagForInt"), &Z_Registration_Info_UScriptStruct_SessionFlagForInt, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FSessionFlagForInt), 4051478677U) },
		{ FSessionFlagForString::StaticStruct, Z_Construct_UScriptStruct_FSessionFlagForString_Statics::NewStructOps, TEXT("SessionFlagForString"), &Z_Registration_Info_UScriptStruct_SessionFlagForString, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FSessionFlagForString), 608991145U) },
		{ FSessionFlagSettings::StaticStruct, Z_Construct_UScriptStruct_FSessionFlagSettings_Statics::NewStructOps, TEXT("SessionFlagSettings"), &Z_Registration_Info_UScriptStruct_SessionFlagSettings, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FSessionFlagSettings), 4130689200U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_gamecreating_PROJECT_MultiPlayerSystem_Plugins_MultiplayerSystemForBP_Source_MultiplayerSystemForBP_Public_BlueprintDataDefinition_h_2709554923(TEXT("/Script/MultiplayerSystemForBP"),
		nullptr, 0,
		Z_CompiledInDeferFile_FID_gamecreating_PROJECT_MultiPlayerSystem_Plugins_MultiplayerSystemForBP_Source_MultiplayerSystemForBP_Public_BlueprintDataDefinition_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_gamecreating_PROJECT_MultiPlayerSystem_Plugins_MultiplayerSystemForBP_Source_MultiplayerSystemForBP_Public_BlueprintDataDefinition_h_Statics::ScriptStructInfo),
		Z_CompiledInDeferFile_FID_gamecreating_PROJECT_MultiPlayerSystem_Plugins_MultiplayerSystemForBP_Source_MultiplayerSystemForBP_Public_BlueprintDataDefinition_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_gamecreating_PROJECT_MultiPlayerSystem_Plugins_MultiplayerSystemForBP_Source_MultiplayerSystemForBP_Public_BlueprintDataDefinition_h_Statics::EnumInfo));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
