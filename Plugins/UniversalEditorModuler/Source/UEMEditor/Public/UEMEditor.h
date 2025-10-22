#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FUEMToolkit;

class FUEMEditorModule : public IModuleInterface, public IHasMenuExtensibility, public IHasToolBarExtensibility
{
private:
    TSharedPtr<FExtensibilityManager> MenuExtensibilityManager;
    
    TSharedPtr<FExtensibilityManager> ToolBarExtensibilityManager;
public:
    virtual void StartupModule() override;
    
    virtual void ShutdownModule() override;

    /** Gets the extensibility managers for outside entities to extend static mesh editor's menus and toolbars */
    virtual TSharedPtr<FExtensibilityManager> GetMenuExtensibilityManager() override { return MenuExtensibilityManager; }
    
    virtual TSharedPtr<FExtensibilityManager> GetToolBarExtensibilityManager() override { return ToolBarExtensibilityManager; }
    
    /** Creates an instance of Behavior Tree editor.  Only virtual so that it can be called across the DLL boundary. */
    virtual TSharedRef<FUEMToolkit> CreateEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* Object);

};
