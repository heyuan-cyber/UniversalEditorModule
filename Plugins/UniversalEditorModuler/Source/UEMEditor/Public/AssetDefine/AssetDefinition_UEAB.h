// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Script/AssetDefinition_Blueprint.h"

#include "AssetDefinition_UEAB.generated.h"

/**
 * 
 */
UCLASS()
class UEMEDITOR_API UAssetDefinition_UEAB : public UAssetDefinition_Blueprint
{
	GENERATED_BODY()
public:
	// UAssetDefinition Implementation
	virtual FText GetAssetDisplayName() const override;
	
	virtual FText GetAssetDisplayName(const FAssetData& AssetData) const override;
	
	virtual FLinearColor GetAssetColor() const override;
	
	virtual TSoftClassPtr<UObject> GetAssetClass() const override;

	virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override;

	virtual EAssetCommandResult PerformAssetDiff(const FAssetDiffArgs& DiffArgs) const override;

	virtual void BuildFilters(TArray<FAssetFilterData>& OutFilters) const override;
	
	virtual EAssetCommandResult OpenAssets(const FAssetOpenArgs& OpenArgs) const override;
	// UAssetDefinition End

	// UAssetDefinition_Blueprint Implementation
	virtual UFactory* GetFactoryForBlueprintType(UBlueprint* InBlueprint) const override;
	// UAssetDefinition_Blueprint End
};
