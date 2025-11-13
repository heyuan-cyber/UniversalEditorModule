#pragma once

#include "CoreMinimal.h"

class SSNMontageSetClipWidget;
class SSNAnimTimeRulerWidget;
class USNMontageSetAsset;
class FSNMontageSetPreviewScene;
class SSNMontageClipViewport;
class SSNMontageSetScrubPanel;

class SCENARIONARRATIVEEDITOR_API FSNMontageSetEditorToolkit : public FAssetEditorToolkit
{
public:
    FSNMontageSetEditorToolkit();
    virtual ~FSNMontageSetEditorToolkit() override;

    void Initialize(const TArray<UObject*>& InObjects, const TSharedPtr<IToolkitHost>& EditWithinLevelEditor);
    void SetAsset(USNMontageSetAsset* InAsset);

    virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
    virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

    virtual FName GetToolkitFName() const override { return "SNMontageSetToolkit"; }
    virtual FText GetBaseToolkitName() const override { return INVTEXT("SNMontage Set Info Asset Editor Toolkit"); }
    virtual FString GetWorldCentricTabPrefix() const override { return TEXT("SNMontageSetEditor"); }
    virtual FLinearColor GetWorldCentricTabColorScale() const override { return FColor::Silver; }
    virtual void InitToolMenuContext(FToolMenuContext& MenuContext) override;

private:
    USNMontageSetAsset* CurrentAsset = nullptr;
    TSharedPtr<IDetailsView> DetailsTab;
    TSharedPtr<FSNMontageSetPreviewScene> PreviewScene;
    TSharedPtr<SSNMontageClipViewport> Viewport;

    TSharedPtr<SSNAnimTimeRulerWidget> TimeRulerControl;
    TSharedPtr<SSNMontageSetClipWidget> ClipSettings;

    static TSharedRef<FTabManager::FLayout> CreateDefaultLayout();
    TSharedPtr<FSNMontageSetPreviewScene> CreatePreviewScene();
    void FocusViewport() const;

    TObjectPtr<USNMontageSetAsset> GetAsset() const { return CurrentAsset; }

protected:
    virtual void OnClose() override;
};
