#pragma once

#include "CoreMinimal.h"
#include "SNTrackWidget.h"

class USNNodeBase;
class SSNTrackWidget;

class SSNNodeWidget : public SCompoundWidget
{
    friend class FSNNodeMoveOp;
    friend class FSNResizeOp;

public:
    enum class EDragType
    {
        None,
        Move,
        ResizeLeft,
        ResizeRight
    };

    SLATE_BEGIN_ARGS(SSNNodeWidget) {}
        SLATE_ARGUMENT(TObjectPtr<USNNodeBase>, NodeData)
        SLATE_ARGUMENT(TSharedPtr<FSNSequencerContext>, Context)
    SLATE_END_ARGS()

    static const float FLAG_COLOR_HEIGHT;
    static const float MAX_RESIZE_BUTTON_WIDTH;

    EDragType CurrentDragType;

    void Construct(const FArguments& InArgs);
    virtual ~SSNNodeWidget() override{UE_LOG(LogTemp, Warning, TEXT("SN node free"));}

    virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    virtual FReply OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

    virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
                          FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
                          bool bParentEnabled) const override;

    virtual void Repaint();

private:
    bool bIsSelected = false;

    TSharedPtr<SOverlay> Slot;
    TSharedPtr<FSNSequencerContext> Context;
    // TSharedPtr<SSNTrackWidget> AttachedTrackWidget;
    TSharedPtr<SButton> ResizeLeftHandle;
    TSharedPtr<SButton> ResizeRightHandle;

    TObjectPtr<USNNodeBase> Data;

    void OnResizeHovered() { SetCursor(EMouseCursor::ResizeLeftRight); }
    void OnResizeUnHovered() { SetCursor(EMouseCursor::Default); }
    void OnOperateDone();

    FReply DetectDrag(EDragType DragType)
    {
        CurrentDragType = DragType;
        return FReply::Handled().DetectDrag(AsShared(), EKeys::LeftMouseButton);
    }

    FReply DetectDrag(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent, EDragType DragType);
};
