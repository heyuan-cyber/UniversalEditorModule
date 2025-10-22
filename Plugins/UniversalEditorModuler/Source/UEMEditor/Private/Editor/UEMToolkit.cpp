// Fill out your copyright notice in the Description page of Project Settings.


#include "Editor/UEMToolkit.h"

#include "Config/Menu/MenuConfig.h"

void FUEMToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FWorkflowCentricApplication::RegisterTabSpawners(InTabManager);
}

void FUEMToolkit::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost,
	UObject* InObject)
{
	TArray<UObject*> ObjectsToEdit;
	if(InObject != nullptr)
	{
		ObjectsToEdit.Add(InObject);
	}
	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;
	InitAssetEditor( Mode, InitToolkitHost, TEXT("UEMEditorApp"), FTabManager::FLayout::NullLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, ObjectsToEdit );

	TSharedPtr<FMenuConfig> MenuConfig = MakeShared<FMenuConfig>();
	MenuConfig->Register();
	BindCommands();
	
}

FName FUEMToolkit::GetToolkitFName() const
{
	return {};
}

FText FUEMToolkit::GetBaseToolkitName() const
{
	return {};
}

FString FUEMToolkit::GetWorldCentricTabPrefix() const
{
	return {};
}

FLinearColor FUEMToolkit::GetWorldCentricTabColorScale() const
{
	return {};
}

FText FUEMToolkit::GetToolkitName() const
{
	return FWorkflowCentricApplication::GetToolkitName();
}

FText FUEMToolkit::GetToolkitToolTipText() const
{
	return FWorkflowCentricApplication::GetToolkitToolTipText();
}

void FUEMToolkit::PostUndo(bool bSuccess)
{
	FEditorUndoClient::PostUndo(bSuccess);
}

void FUEMToolkit::PostRedo(bool bSuccess)
{
	FEditorUndoClient::PostRedo(bSuccess);
}

void FUEMToolkit::NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent, FProperty* PropertyThatChanged)
{
	FNotifyHook::NotifyPostChange(PropertyChangedEvent, PropertyThatChanged);
}

void FUEMToolkit::BindCommands()
{
	
}
