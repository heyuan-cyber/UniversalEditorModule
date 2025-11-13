#pragma once
#include "Modules/ModuleManager.h"

class FScenarioNarrativeEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;

    virtual void ShutdownModule() override;
};


//IMPLEMENT_MODULE(FScenarioNarrativeEditorModule, ScenarioNarrativeEditor);
