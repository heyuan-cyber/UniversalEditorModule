// Fill out your copyright notice in the Description page of Project Settings.


#include "Editor/UMEToolkit.h"

#include "Config/Menu/MenuConfig.h"

void FUMEToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FWorkflowCentricApplication::RegisterTabSpawners(InTabManager);
}

void FUMEToolkit::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost,
	UObject* InObject)
{
	TArray<UObject*> ObjectsToEdit;
	if(InObject != nullptr)
	{
		ObjectsToEdit.Add(InObject);
	}
	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;
	InitAssetEditor( Mode, InitToolkitHost, TEXT("UMEEditorApp"), FTabManager::FLayout::NullLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, ObjectsToEdit );

	TSharedPtr<FMenuConfig> MenuConfig = MakeShared<FMenuConfig>();
	MenuConfig->Register();
	BindCommands();
	
}

FName FUMEToolkit::GetToolkitFName() const
{
	return {};
}

FText FUMEToolkit::GetBaseToolkitName() const
{
	return {};
}

FString FUMEToolkit::GetWorldCentricTabPrefix() const
{
	return {};
}

FLinearColor FUMEToolkit::GetWorldCentricTabColorScale() const
{
	return {};
}

FText FUMEToolkit::GetToolkitName() const
{
	return FWorkflowCentricApplication::GetToolkitName();
}

FText FUMEToolkit::GetToolkitToolTipText() const
{
	return FWorkflowCentricApplication::GetToolkitToolTipText();
}

void FUMEToolkit::PostUndo(bool bSuccess)
{
	FEditorUndoClient::PostUndo(bSuccess);
}

void FUMEToolkit::PostRedo(bool bSuccess)
{
	FEditorUndoClient::PostRedo(bSuccess);
}

void FUMEToolkit::NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent, FProperty* PropertyThatChanged)
{
	FNotifyHook::NotifyPostChange(PropertyChangedEvent, PropertyThatChanged);
}

void FUMEToolkit::BindCommands()
{
	
}
