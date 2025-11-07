// Fill out your copyright notice in the Description page of Project Settings.

#include "AssetDefine/UEAFactory.h"
#include "Asset/UniversalExampleAsset.h"

#define LOCTEXT_NAMESPACE "UEAFactory"

UUEAFactory::UUEAFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UUniversalExampleAsset::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UUEAFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags,
	UObject* Context, FFeedbackContext* Warn)
{
	check(Class->IsChildOf(UUniversalExampleAsset::StaticClass()));
	return NewObject<UUniversalExampleAsset>(InParent, Class, Name, Flags);
}

bool UUEAFactory::CanCreateNew() const
{
	return true;
}

#undef LOCTEXT_NAMESPACE