// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "MultiplayerSystemForBP/Public/MyGameSession.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMyGameSession() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_AGameSession();
	MULTIPLAYERSYSTEMFORBP_API UClass* Z_Construct_UClass_AMyGameSession();
	MULTIPLAYERSYSTEMFORBP_API UClass* Z_Construct_UClass_AMyGameSession_NoRegister();
	UPackage* Z_Construct_UPackage__Script_MultiplayerSystemForBP();
// End Cross Module References
	void AMyGameSession::StaticRegisterNativesAMyGameSession()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AMyGameSession);
	UClass* Z_Construct_UClass_AMyGameSession_NoRegister()
	{
		return AMyGameSession::StaticClass();
	}
	struct Z_Construct_UClass_AMyGameSession_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AMyGameSession_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameSession,
		(UObject* (*)())Z_Construct_UPackage__Script_MultiplayerSystemForBP,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AMyGameSession_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "MyGameSession.h" },
		{ "ModuleRelativePath", "Public/MyGameSession.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AMyGameSession_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AMyGameSession>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AMyGameSession_Statics::ClassParams = {
		&AMyGameSession::StaticClass,
		"Game",
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
		METADATA_PARAMS(Z_Construct_UClass_AMyGameSession_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AMyGameSession_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AMyGameSession()
	{
		if (!Z_Registration_Info_UClass_AMyGameSession.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AMyGameSession.OuterSingleton, Z_Construct_UClass_AMyGameSession_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AMyGameSession.OuterSingleton;
	}
	template<> MULTIPLAYERSYSTEMFORBP_API UClass* StaticClass<AMyGameSession>()
	{
		return AMyGameSession::StaticClass();
	}
	AMyGameSession::AMyGameSession(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AMyGameSession);
	AMyGameSession::~AMyGameSession() {}
	struct Z_CompiledInDeferFile_FID_gamecreating_PROJECT_MultiPlayerSystem_Plugins_MultiplayerSystemForBP_Source_MultiplayerSystemForBP_Public_MyGameSession_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_gamecreating_PROJECT_MultiPlayerSystem_Plugins_MultiplayerSystemForBP_Source_MultiplayerSystemForBP_Public_MyGameSession_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AMyGameSession, AMyGameSession::StaticClass, TEXT("AMyGameSession"), &Z_Registration_Info_UClass_AMyGameSession, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AMyGameSession), 3897995392U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_gamecreating_PROJECT_MultiPlayerSystem_Plugins_MultiplayerSystemForBP_Source_MultiplayerSystemForBP_Public_MyGameSession_h_3637347007(TEXT("/Script/MultiplayerSystemForBP"),
		Z_CompiledInDeferFile_FID_gamecreating_PROJECT_MultiPlayerSystem_Plugins_MultiplayerSystemForBP_Source_MultiplayerSystemForBP_Public_MyGameSession_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_gamecreating_PROJECT_MultiPlayerSystem_Plugins_MultiplayerSystemForBP_Source_MultiplayerSystemForBP_Public_MyGameSession_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
