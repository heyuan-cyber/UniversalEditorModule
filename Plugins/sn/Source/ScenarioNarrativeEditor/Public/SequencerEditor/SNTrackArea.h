#pragma once

#include "CoreMinimal.h"
#include "Widget/SNTimeRulerWidget.h"

class USNTrackBase;
class FSNSequencerContext;
class SSNTrackNodes;
class USNSequencer;
class SSNTrackWidget;

class SSNTrackArea : public SCompoundWidget
{
    SLATE_BEGIN_ARGS(SSNTrackArea) { }
        SLATE_ATTRIBUTE(float, SequenceLength)
        SLATE_ATTRIBUTE(uint32, NumOfKeys)
        SLATE_ATTRIBUTE(float, ViewInputMin)
        SLATE_ATTRIBUTE(float, ViewInputMax)
        SLATE_EVENT(FOnSetInputViewRange, OnSetInputViewRange)
        SLATE_EVENT(FOnUserScrolled, OnUserScrolled)
        SLATE_ARGUMENT(TSharedPtr<FSNSequencerContext>, Context)
        SLATE_ARGUMENT(TObjectPtr<USNSequencer>, Asset)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);
    virtual ~SSNTrackArea() override;

    virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    virtual FReply OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
                          FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
                          bool bParentEnabled) const override;

    virtual void Repaint();
    void SetScrollOffset(float Offset) const;
public:
    TSharedPtr<SSNTimeRulerWidget> GetTimeRulerWidget() { return TimeRuler; };
private:
    TSharedPtr<SScrollBox> Layout;
    TSharedPtr<SSNTimeRulerWidget> TimeRuler;
    TSharedPtr<FSNSequencerContext> Context;

    // float Value = 0.f;
    TArray<float> MajorTicks;

    TObjectPtr<USNSequencer> Data;
    FDelegateHandle RemoveTrackHandle;
    FDelegateHandle CreateTrackHandle;

    void OnTrackChanged(TObjectPtr<USNTrackBase> Track);
    void DrawLine(const float& PosX, const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId,
                  bool bIsDashLine = false, FLinearColor LineColor = FLinearColor::White) const;

    void DeleteNode() const;

    void BindCommands();
};
