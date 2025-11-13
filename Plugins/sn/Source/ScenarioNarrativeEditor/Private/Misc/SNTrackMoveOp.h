// SNTrackMoveOp.h
// @author : xiangyp
// @date : 2025/06/06 11:09:01
// @desc : 移动轨道顺序操作

#pragma once
#include "SequencerEditor/SNTrackWidget.h"

class FSNTrackMoveOp : public FDragDropOperation
{
public:
    DRAG_DROP_OPERATOR_TYPE(FTrackDragDropOp, FDragDropOperation)

    FSNTrackMoveOp();
    virtual ~FSNTrackMoveOp() override = default;

    virtual void OnDragged(const FDragDropEvent& DragDropEvent) override;
    virtual void OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent) override;

    static TSharedRef<FSNTrackMoveOp> New(USNTrackBase* InTrackWidget, FSNSequencerContext* InContext, const FSimpleDelegate& InOperateDone);

private:
    FSNSequencerContext* Context;
    USNTrackBase* DragTrack;
    FSimpleDelegate OnOperateDone;

    int32_t Index;
};
