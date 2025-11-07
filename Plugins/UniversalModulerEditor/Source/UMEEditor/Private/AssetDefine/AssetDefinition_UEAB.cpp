// Fill out your copyright notice in the Description page of Project Settings.

#include "AssetDefine/AssetDefinition_UEAB.h"
#include "Asset/UEABlueprint.h"
#include "AssetDefine/UEABlueprintFactory.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

FText UAssetDefinition_UEAB::GetAssetDisplayName() const
{
	return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_UEAB", "UEA Blueprint");
}

FText UAssetDefinition_UEAB::GetAssetDisplayName(const FAssetData& AssetData) const
{
	return Super::GetAssetDisplayName(AssetData);
}

FLinearColor UAssetDefinition_UEAB::GetAssetColor() const
{
	return FLinearColor(FColor(200, 116, 0));
}

TSoftClassPtr<UObject> UAssetDefinition_UEAB::GetAssetClass() const
{
	return UUEABlueprint::StaticClass();
}

TConstArrayView<FAssetCategoryPath> UAssetDefinition_UEAB::GetAssetCategories() const
{
	static const auto Categories = { FAssetCategoryPath(LOCTEXT("UniversalEditorAsset", "Universal Example Asset")) };
	return Categories;
}

EAssetCommandResult UAssetDefinition_UEAB::PerformAssetDiff(const FAssetDiffArgs& DiffArgs) const
{
	return Super::PerformAssetDiff(DiffArgs);
}

void UAssetDefinition_UEAB::BuildFilters(TArray<FAssetFilterData>& OutFilters) const
{
	Super::BuildFilters(OutFilters);
}

EAssetCommandResult UAssetDefinition_UEAB::OpenAssets(const FAssetOpenArgs& OpenArgs) const
{
	return Super::OpenAssets(OpenArgs);
}

UFactory* UAssetDefinition_UEAB::GetFactoryForBlueprintType(UBlueprint* InBlueprint) const
{
	UUEABlueprintFactory* UEABFactory = NewObject<UUEABlueprintFactory>();
	return UEABFactory;
}

#undef LOCTEXT_NAMESPACE