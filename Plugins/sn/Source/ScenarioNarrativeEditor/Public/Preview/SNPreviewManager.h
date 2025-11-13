#pragma once

class FSNAssetPreviewBase;
class USNAssetBase;
class FSNAssetPreviewPerformer;
class FSNAssetPreviewHUD;
class FSNAssetPreviewCamera;
class FSNAssetPreviewSpline;
class FSNAssetPreviewScene;
class SSNAssetViewport;
class USNSequencer;
struct FSNPreviewContext;
class USNNodeBase;

enum class ESNPreviewPlayerState
{
    Stopped,
    Playing,
    Paused
};

class FSNPreviewManager : public FTickableEditorObject, public TSharedFromThis<FSNPreviewManager>
{
public:
    FSNPreviewManager();

    virtual ~FSNPreviewManager();

    float AccumulatedTime;

    float TickInterval;

    float CurrentTime;

    float LastSampleTime;

    ESNPreviewPlayerState State;

    TSet<TObjectPtr<USNNodeBase>> LastSampleNodes;

    TSharedPtr<FSNPreviewContext> PreviewContext;

    static TMap<UClass*, FSNAssetPreviewBase*> NodeToManager;

    void UpdateTickFrameRate(float InTickInterval) { TickInterval = InTickInterval; }

    // implement from FTickableEditorObject
    virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(FAssetPreviewPlayer, STATGROUP_Tickables); }

    virtual bool IsTickable() const override { return true; }

    virtual void Tick(float DeltaTime) override;

    void SampleAtTime(float Time);

    void JumpTo(float NextTime);
};
