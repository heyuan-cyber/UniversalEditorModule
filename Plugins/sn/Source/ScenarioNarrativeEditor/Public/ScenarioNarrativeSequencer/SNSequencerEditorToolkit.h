#pragma once

#include "CoreMinimal.h"

class FSNAssetPreviewPerformer;
class FSNPreviewManager;
class ISceneOutliner;
class FSNAssetPreviewHUD;
class APreviewActor;
class USNTrackBase;
class USNNodeBase;
class FSNAssetPreviewScene;
class FSNAssetPreviewCamera;
class FSNAssetPreviewSpline;
class FSNSequencerContext;
class USNSequencer;
class SSNAssetViewport;
class SSNSequencerWidget;

class SCENARIONARRATIVEEDITOR_API FSNSequencerEditorToolkit : public FAssetEditorToolkit, public FNotifyHook, public FTickableEditorObject
{
public:
    FSNSequencerEditorToolkit();
    virtual ~FSNSequencerEditorToolkit() override;

    void InitAssetEditor(const EToolkitMode::Type InMode, const TSharedPtr<IToolkitHost>& InitToolkitHost,
        const TObjectPtr<USNSequencer>& InAsset);

    virtual void Tick(float DeltaTime) override;
    virtual TStatId GetStatId() const override;
    virtual bool IsTickable() const override;

#pragma region Virtual Functions From Super

    virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
    virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

    virtual void InitToolMenuContext(FToolMenuContext& MenuContext) override;
    // virtual void CreateEditorModeManager() override;

    virtual FName GetToolkitFName() const override { return "SNSequencerEditorToolkit"; }
    virtual FText GetBaseToolkitName() const override { return FText::FromString(TEXT("SN Sequencer Editor Toolkit")); }
    virtual FString GetWorldCentricTabPrefix() const override { return "SN Sequencer"; }
    virtual FLinearColor GetWorldCentricTabColorScale() const override { return {}; }

    // inherited from FNotifyHook
    virtual void NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent, FProperty* PropertyThatChanged) override;

    TSharedPtr<SSNAssetViewport> GetViewport() { return ViewportWidget; }

    TSharedPtr<FSNAssetPreviewScene> GetPreviewScene() { return PreviewScene; }

    TObjectPtr<USNSequencer> GetSequenceWidget() { return Target; }

#pragma endregion

private:
    TObjectPtr<USNSequencer> Target;
    TSharedPtr<FSNSequencerContext> Context;

    TSharedPtr<SSNSequencerWidget> SNSequencerWidget;
    TSharedPtr<IDetailsView> InspectorWidget;
    TSharedPtr<SSNAssetViewport> ViewportWidget;
    TSharedPtr<ISceneOutliner> OutlinerWidget;

    TSharedPtr<FSNPreviewManager> PreviewManager;
    TSharedPtr<FSNAssetPreviewScene> PreviewScene;
    TSharedPtr<FSNAssetPreviewCamera> PreviewCamera;
    TSharedPtr<FSNAssetPreviewSpline> PreviewSpline;
    TSharedPtr<FSNAssetPreviewHUD> PreviewHUD;
    TSharedPtr<FSNAssetPreviewPerformer> PreviewPerformer;

    double LastTickTime = 0.0;

    TSharedRef<FTabManager::FLayout> CreateDefaultLayout() const;
    void HandleObjectPendingDelete(const TArray<UObject*>& ObjectsForDelete);
    TSharedRef<FSNAssetPreviewScene> MakePreviewScene();

    bool ShouldHaveMainCamera() const;
    void UpdatePreviewResource() const;
    void OnActorPicked(AActor* PickedActor);

    void OnNodeCreated(TObjectPtr<USNNodeBase> InNode) const;
    void OnNodeDeleted(TObjectPtr<USNNodeBase> InNode) const;
    void OnTrackDeleted(TObjectPtr<USNTrackBase> InTrack) const;

    void ExportAssetToJson();

    // void OnTransSNChanged(const FTransSNContext& TransSNContext, ETransSNStateEventType State);
    void OnUndo(const FTransactionContext& TransSNContext, bool Succeeded);
    void OnRedo(const FTransactionContext& TransSNContext, bool Succeeded);
public:
    void OnElementSelectionChanged(const UTypedElementSelectionSet* SelectionSet);
};
