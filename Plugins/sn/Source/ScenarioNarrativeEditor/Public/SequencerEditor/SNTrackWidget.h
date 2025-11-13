#pragma once

#include "CoreMinimal.h"

class USNTrackBase;
class STimeRulerWidget;
class FSNSequencerContext;

class SSNTrackWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SSNTrackWidget) {}
        SLATE_ARGUMENT(TObjectPtr<USNTrackBase>, TrackData)
        SLATE_ARGUMENT(TSharedPtr<FSNSequencerContext>, Context)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);
    virtual ~SSNTrackWidget() override {UE_LOG(LogTemp, Warning, TEXT("track widget free"));}

    virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
                          FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
                          bool bParentEnabled) const override;
    virtual FReply OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    virtual void Repaint();

    void SetIsDrop(bool InIsDrop) { bIsDrop = InIsDrop; }

protected:
    bool bIsDrop = false;
    TSharedPtr<SOverlay> Slot;
    TSharedPtr<FSNSequencerContext> Context;

    TObjectPtr<USNTrackBase> Data;
};
