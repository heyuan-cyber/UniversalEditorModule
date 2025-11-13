#pragma once

#include "CoreMinimal.h"
#include "ITransportControl.h"

class FAssetPreviewPlayer;
class FSNSequencerContext;
class SSNTrackTimeline;
class USNSequencer;
class ITransportControl;

UENUM()
enum class ESNFrameRate : uint8
{
    FPS_24,
    FPS_30,
    FPS_60,
    FPS_120
};

class SSNSequencerWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SSNSequencerWidget) {}
        SLATE_ARGUMENT(TSharedPtr<FSNSequencerContext>, Context)
        SLATE_ARGUMENT(TObjectPtr<USNSequencer>, Asset)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);
    virtual ~SSNSequencerWidget() override {UE_LOG(LogTemp, Warning, TEXT("SSNSequencerWidget free"));}

    virtual bool SupportsKeyboardFocus() const override { return true; }
    TSharedRef<SWidget> GenerateFrameRateMenu();
    FString GetFrameRateText(ESNFrameRate Rate) const;
    ESNFrameRate GetFrameRateEnumFromUint32(uint32 FrameRate) const;
    uint32 GetFrameRateUint32(ESNFrameRate Rate) const;
    bool IsFrameRateChecked(ESNFrameRate Rate) const;
    void SetFrameRate(ESNFrameRate NewRate);
    virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
private:
    TSharedPtr<ITransportControl> TransportControl;
    TSharedPtr<SSNTrackTimeline> TrackTimeline;
    TSharedPtr<FSNSequencerContext> Context;
    ESNFrameRate CurrentFrameRate = ESNFrameRate::FPS_30;

    TSharedRef<ITransportControl> MakeTransportControls();
    FReply TogglePlayback();
    FReply StopPlayback();
    FReply ToggleLooping();
    bool IsLooping() const;
    FReply StepToNextFrame();
    FReply StepToPreviousFrame();
    EPlaybackMode::Type GetPlaybackMode() const;
};
