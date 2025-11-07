// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetDefine/UEABlueprintFactory.h"
#include "Asset/UEABlueprint.h"
#include "Asset/UniversalExampleAsset.h"

#define LOCTEXT_NAMESPACE "UEABlueprintFactory"

UUEABlueprintFactory::UUEABlueprintFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UUEABlueprint::StaticClass();
}

bool UUEABlueprintFactory::ConfigureProperties()
{
	return true;
}

UObject* UUEABlueprintFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
                                                UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	check(InClass->IsChildOf(UUEABlueprint::StaticClass()));

	const auto NewBP = NewObject<UUEABlueprint>(InParent, InClass, InName, Flags);

	NewBP->ParentClass = UUniversalExampleAsset::StaticClass();
	NewBP->BlueprintType = BPTYPE_Normal;
	NewBP->bIsNewlyCreated = true;
	NewBP->bLegacyNeedToPurgeSkelRefs = false;
	NewBP->GenerateNewGuid();

	return NewBP;
}

UObject* UUEABlueprintFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags,
	UObject* Context, FFeedbackContext* Warn)
{
	return Super::FactoryCreateNew(Class, InParent, Name, Flags, Context, Warn);
}
#undef LOCTEXT_NAMESPACE