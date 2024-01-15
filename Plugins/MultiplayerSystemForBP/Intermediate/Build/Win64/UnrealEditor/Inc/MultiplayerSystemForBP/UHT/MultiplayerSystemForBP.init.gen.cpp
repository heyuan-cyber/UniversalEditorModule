// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMultiplayerSystemForBP_init() {}
	MULTIPLAYERSYSTEMFORBP_API UFunction* Z_Construct_UDelegateFunction_MultiplayerSystemForBP_AsyncResultDelegate__DelegateSignature();
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_MultiplayerSystemForBP;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_MultiplayerSystemForBP()
	{
		if (!Z_Registration_Info_UPackage__Script_MultiplayerSystemForBP.OuterSingleton)
		{
			static UObject* (*const SingletonFuncArray[])() = {
				(UObject* (*)())Z_Construct_UDelegateFunction_MultiplayerSystemForBP_AsyncResultDelegate__DelegateSignature,
			};
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/MultiplayerSystemForBP",
				SingletonFuncArray,
				UE_ARRAY_COUNT(SingletonFuncArray),
				PKG_CompiledIn | 0x00000000,
				0x2D02CF96,
				0x0331CE14,
				METADATA_PARAMS(nullptr, 0)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_MultiplayerSystemForBP.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_MultiplayerSystemForBP.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_MultiplayerSystemForBP(Z_Construct_UPackage__Script_MultiplayerSystemForBP, TEXT("/Script/MultiplayerSystemForBP"), Z_Registration_Info_UPackage__Script_MultiplayerSystemForBP, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0x2D02CF96, 0x0331CE14));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
