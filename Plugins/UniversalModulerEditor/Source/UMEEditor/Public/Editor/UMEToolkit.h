// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"


/**
 * 
 */

class UMEEDITOR_API FUMEToolkit : public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook
{
#pragma region Vitual Function

public:
	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;


	virtual void InitEditor(const EToolkitMode::Type Mode,
	                        const TSharedPtr<class IToolkitHost>& InitToolkitHost, UObject* InObject);

	//~ Begin IToolkit Interface
	virtual FName GetToolkitFName() const override;

	virtual FText GetBaseToolkitName() const override;

	virtual FString GetWorldCentricTabPrefix() const override;

	virtual FLinearColor GetWorldCentricTabColorScale() const override;

	virtual FText GetToolkitName() const override;

	virtual FText GetToolkitToolTipText() const override;
	//~ End IToolkit Interface

	//~ Begin FEditorUndoClient Interface
	virtual void PostUndo(bool bSuccess) override;

	virtual void PostRedo(bool bSuccess) override;
	// End of FEditorUndoClient

	//~ Begin FNotifyHook Interface
	virtual void NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent,
	                              FProperty* PropertyThatChanged) override;
	// End of FNotifyHook
#pragma endregion

	void BindCommands();
};
