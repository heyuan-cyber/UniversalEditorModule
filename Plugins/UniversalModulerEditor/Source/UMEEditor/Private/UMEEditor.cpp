#include "UMEEditor.h"

#include "Editor/UMEToolkit.h"

#define LOCTEXT_NAMESPACE "FUMEEditorModule"

void FUMEEditorModule::StartupModule()
{
    
}

void FUMEEditorModule::ShutdownModule()
{
    
}

TSharedRef<FUMEToolkit> FUMEEditorModule::CreateEditor(const EToolkitMode::Type Mode,
	const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* Object)
{
	TSharedRef<FUMEToolkit> NewEditor(new FUMEToolkit());
	NewEditor->InitEditor(Mode, InitToolkitHost, Object);
	return NewEditor;
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FUMEEditorModule, UMEEditor)