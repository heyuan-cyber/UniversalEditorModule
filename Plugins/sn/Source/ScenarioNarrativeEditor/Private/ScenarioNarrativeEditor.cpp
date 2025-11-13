#include "ScenarioNarrativeEditor.h"
#include "CoreMinimal.h"
#include "EditorModeRegistry.h"
#include "SNHotkeys.h"
#include "ScenarioNarrativeEditorDefines.h"
#include "SNTypeRegistryHelper.h"
#include "EditorEngine/EditorMode/SNEditorMode.h"


DEFINE_LOG_CATEGORY(LogScenarioNarrativeEditor);

#define LOCTEXT_NAMESPACE "FScenarioNarrativeEditorModule"

void FScenarioNarrativeEditorModule::StartupModule()
{
    IModuleInterface::StartupModule();

    const auto& Delegate = FSimpleMulticastDelegate::FDelegate::CreateStatic(&FSNTypeRegistryHelper::RegisterStartupCallback);
    UToolMenus::RegisterStartupCallback(Delegate);
    FSNTypeRegistryHelper::Get().Initialize();
    FSNHotkeys::Register();

    USNEditorMode* MyMode = NewObject<USNEditorMode>(GetTransientPackage(), USNEditorMode::StaticClass());

}

void FScenarioNarrativeEditorModule::ShutdownModule()
{
    IModuleInterface::ShutdownModule();
    UToolMenus::UnRegisterStartupCallback("FSNTypeRegistryHelper");
    FSNHotkeys::Unregister();

    // unregister edit mode
    FEditorModeRegistry::Get().UnregisterMode(USNEditorMode::EdModeId);
}

#undef LOCTEXT_NAMESPACE
IMPLEMENT_MODULE(FScenarioNarrativeEditorModule, ScenarioNarrativeEditor)
