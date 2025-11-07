// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AssetDefinitionDefault.h"
#include "AssetDefinition_UniversalExampleAsset.generated.h"

/**
 * 
 */
UCLASS()
class UMEEDITOR_API UAssetDefinition_UniversalExampleAsset : public UAssetDefinitionDefault
{
	GENERATED_BODY()
	
protected:
	// UAssetDefinition Begin
	virtual FText GetAssetDisplayName() const override;
	
	virtual FLinearColor GetAssetColor() const override;
	
	virtual TSoftClassPtr<UObject> GetAssetClass() const override;
	
	virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override;
	
	virtual EAssetCommandResult OpenAssets(const FAssetOpenArgs& OpenArgs) const override;
	
	virtual EAssetCommandResult PerformAssetDiff(const FAssetDiffArgs& DiffArgs) const override;
	// UAssetDefinition End
};
