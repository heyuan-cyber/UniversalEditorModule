// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "UEABlueprintFactory.generated.h"

/**
 * 
 */
class UUniversalExampleAsset;
UCLASS()
class UEMEDITOR_API UUEABlueprintFactory : public UFactory
{
	GENERATED_UCLASS_BODY()
	
public:
	//~ Begin UFactory Interface
	virtual bool ConfigureProperties() override;
	
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	
	virtual UObject* FactoryCreateNew(UClass* Class,UObject* InParent,FName Name,EObjectFlags Flags,UObject* Context,FFeedbackContext* Warn) override;
	//~ Begin UFactory Interface	
};
