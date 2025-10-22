#include "UEMEditor.h"

#include "Editor/UEMToolkit.h"

#define LOCTEXT_NAMESPACE "FUEMEditorModule"

void FUEMEditorModule::StartupModule()
{
    
}

void FUEMEditorModule::ShutdownModule()
{
    
}

TSharedRef<FUEMToolkit> FUEMEditorModule::CreateEditor(const EToolkitMode::Type Mode,
	const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* Object)
{
	TSharedRef<FUEMToolkit> NewEditor(new FUEMToolkit());
	NewEditor->InitEditor(Mode, InitToolkitHost, Object);
	return NewEditor;
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FUEMEditorModule, UEMEditor)