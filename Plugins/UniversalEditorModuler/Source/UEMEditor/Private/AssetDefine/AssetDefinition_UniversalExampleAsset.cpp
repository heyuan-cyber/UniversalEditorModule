// Fill out your copyright notice in the Description page of Project Settings.

#include "AssetDefine/AssetDefinition_UniversalExampleAsset.h"

#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "UEMEditor.h"
#include "Asset/UniversalExampleAsset.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

FText UAssetDefinition_UniversalExampleAsset::GetAssetDisplayName() const
{
	return LOCTEXT("AssetTypeActions_UniversalExampleAsset", "Universal Example Asset");
}

FLinearColor UAssetDefinition_UniversalExampleAsset::GetAssetColor() const
{
	return FColor(149,70,255);
}

TSoftClassPtr<UObject> UAssetDefinition_UniversalExampleAsset::GetAssetClass() const
{
	return UUniversalExampleAsset::StaticClass();
}

TConstArrayView<FAssetCategoryPath> UAssetDefinition_UniversalExampleAsset::GetAssetCategories() const
{
	static TArray<FAssetCategoryPath> Categories;
	
	if (Categories.Num() == 0)
	{
		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	
		// 注册分类（返回一个枚举值）
		EAssetTypeCategories::Type CategoryBit = AssetTools.RegisterAdvancedAssetCategory(
			TEXT("UniversalEditorAsset"),
			FText::FromString(TEXT("Universal Example Asset"))
		);
	
		// 构造 FAssetCategoryPath
		Categories.Add(FAssetCategoryPath(FText::FromString(TEXT("Universal Example Asset"))));
	}
	return Categories;
}

EAssetCommandResult UAssetDefinition_UniversalExampleAsset::OpenAssets(const FAssetOpenArgs& OpenArgs) const
{
	//return Super::OpenAssets(OpenArgs);
	const EToolkitMode::Type Mode = OpenArgs.GetToolkitMode();

	for (UUniversalExampleAsset* UEA : OpenArgs.LoadObjects<UUniversalExampleAsset>())
	{
		// check if we have an editor open for this BT's blackboard & use that if we can
		bool bFoundExisting = false;
		
		if (!bFoundExisting)
		{
			FUEMEditorModule& UEMEditorModule = FModuleManager::GetModuleChecked<FUEMEditorModule>("UEMEditor");
			UEMEditorModule.CreateEditor(Mode, OpenArgs.ToolkitHost, UEA);
		}
	}
	return EAssetCommandResult::Handled;
}

EAssetCommandResult UAssetDefinition_UniversalExampleAsset::PerformAssetDiff(const FAssetDiffArgs& DiffArgs) const
{
	return Super::PerformAssetDiff(DiffArgs);
}

#undef LOCTEXT_NAMESPACE