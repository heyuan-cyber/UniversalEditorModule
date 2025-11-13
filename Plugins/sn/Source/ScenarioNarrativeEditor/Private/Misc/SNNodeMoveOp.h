// SNNodeMoveOp.h
// @author : xiangyp
// @date : 2025/04/25 10:50:32
// @desc : 拖拽操作

#pragma once

class USNTrackBase;
class SSNNodeWidget;

class FSNNodeMoveOp : public FDragDropOperation
{
public:
    DRAG_DROP_OPERATOR_TYPE(FNodeDragDropOp, FDragDropOperation)

    FSNNodeMoveOp();
    virtual ~FSNNodeMoveOp() override = default;

    virtual void OnDragged(const FDragDropEvent& DragDropEvent) override;
    virtual void OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent) override;

    static TSharedRef<FSNNodeMoveOp> New(const TSharedRef<SSNNodeWidget>& InNodeWidget, float InOffset,
                                             const TObjectPtr<USNTrackBase>& InTrackData, const FSimpleDelegate& InOperateDone);

private:
    float Offset = 0.f;
    FVector2D RawTime;

    TSharedPtr<SSNNodeWidget> NodeWidget;
    TObjectPtr<USNTrackBase> RawTrack;
    TObjectPtr<USNTrackBase> LastSavedTrack;

    FSimpleDelegate OnOperateDone;
};
