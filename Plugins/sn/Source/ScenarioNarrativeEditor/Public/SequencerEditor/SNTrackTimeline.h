#pragma once

#include "CoreMinimal.h"

class FSNSequencerContext;
class SSNTrackNodes;
class SSNTrackWidget;
class USNSequencer;
class SSNTrackArea;
class SSNTrackHierarchy;
class FAssetPreviewPlayer;

class SSNTrackTimeline : public SCompoundWidget
{
    SLATE_BEGIN_ARGS(SSNTrackTimeline) {}
        SLATE_ARGUMENT(TObjectPtr<USNSequencer>, Asset)
        SLATE_ARGUMENT(TSharedPtr<FSNSequencerContext>, Context)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);
    virtual ~SSNTrackTimeline() override { UE_LOG(LogTemp, Warning, TEXT("timeline free")); }

    virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    virtual FReply OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
public:
    TSharedPtr<SSNTrackArea> GetTrackAreaWidget() { return TrackArea; };
private:
    const float EXTRA_LENGTH_RATE = 1.2f;

    float ViewMinVal;
    float ViewMaxVal;
    float SequenceLength;

    TSharedPtr<SSNTrackHierarchy> TrackHierarchy;
    TSharedPtr<SSNTrackArea> TrackArea;

    TSharedPtr<SSNTrackWidget> HoveredTrackWidget;
    TSharedPtr<SSNTrackNodes> HoveredNodesWidget;

    TObjectPtr<USNSequencer> Data;
    TSharedPtr<FSNSequencerContext> Context;

    void OnSetInputViewRange(float InMinVal, float InMaxVal);
    float GetViewMin() const { return ViewMinVal; }
    float GetViewMax() const { return ViewMaxVal; }
    uint32 GetNOK() const;
    uint32 GetMaxKeyCounts() const;
    float GetSequenceLength() const { return SequenceLength; }

    void OnTrackScroll(float Offset) const;

    void UpdateHoverTrack(const FVector2D& CursorPos) const;
    void UpdateHoverNode(const FVector2D& CursorPos) const;
};
