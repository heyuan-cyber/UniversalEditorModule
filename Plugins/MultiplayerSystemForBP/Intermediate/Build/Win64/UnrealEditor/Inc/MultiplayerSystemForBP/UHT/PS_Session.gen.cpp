// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "MultiplayerSystemForBP/Public/PS_Session.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePS_Session() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_APlayerState();
	MULTIPLAYERSYSTEMFORBP_API UClass* Z_Construct_UClass_APS_Session();
	MULTIPLAYERSYSTEMFORBP_API UClass* Z_Construct_UClass_APS_Session_NoRegister();
	UPackage* Z_Construct_UPackage__Script_MultiplayerSystemForBP();
// End Cross Module References
	void APS_Session::StaticRegisterNativesAPS_Session()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(APS_Session);
	UClass* Z_Construct_UClass_APS_Session_NoRegister()
	{
		return APS_Session::StaticClass();
	}
	struct Z_Construct_UClass_APS_Session_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_APS_Session_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_APlayerState,
		(UObject* (*)())Z_Construct_UPackage__Script_MultiplayerSystemForBP,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_APS_Session_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "PS_Session.h" },
		{ "ModuleRelativePath", "Public/PS_Session.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_APS_Session_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<APS_Session>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_APS_Session_Statics::ClassParams = {
		&APS_Session::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009002A4u,
		METADATA_PARAMS(Z_Construct_UClass_APS_Session_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_APS_Session_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_APS_Session()
	{
		if (!Z_Registration_Info_UClass_APS_Session.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_APS_Session.OuterSingleton, Z_Construct_UClass_APS_Session_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_APS_Session.OuterSingleton;
	}
	template<> MULTIPLAYERSYSTEMFORBP_API UClass* StaticClass<APS_Session>()
	{
		return APS_Session::StaticClass();
	}
	APS_Session::APS_Session(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(APS_Session);
	APS_Session::~APS_Session() {}
	struct Z_CompiledInDeferFile_FID_gamecreating_PROJECT_MultiPlayerSystem_Plugins_MultiplayerSystemForBP_Source_MultiplayerSystemForBP_Public_PS_Session_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_gamecreating_PROJECT_MultiPlayerSystem_Plugins_MultiplayerSystemForBP_Source_MultiplayerSystemForBP_Public_PS_Session_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_APS_Session, APS_Session::StaticClass, TEXT("APS_Session"), &Z_Registration_Info_UClass_APS_Session, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(APS_Session), 4163519006U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_gamecreating_PROJECT_MultiPlayerSystem_Plugins_MultiplayerSystemForBP_Source_MultiplayerSystemForBP_Public_PS_Session_h_3440307910(TEXT("/Script/MultiplayerSystemForBP"),
		Z_CompiledInDeferFile_FID_gamecreating_PROJECT_MultiPlayerSystem_Plugins_MultiplayerSystemForBP_Source_MultiplayerSystemForBP_Public_PS_Session_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_gamecreating_PROJECT_MultiPlayerSystem_Plugins_MultiplayerSystemForBP_Source_MultiplayerSystemForBP_Public_PS_Session_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
