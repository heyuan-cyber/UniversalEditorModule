// SNResizeOp.h
// @author : xiangyp
// @date : 2025/04/25 21:40:08
// @desc : 片段大小拖拽操作

#pragma once

#include "SequencerEditor/SNNodeWidget.h"

class SSNNodeWidget;

class FSNResizeOp : public FDragDropOperation
{
    DRAG_DROP_OPERATOR_TYPE(FSNResizeOp, FDragDropOperation)

public:
    FSNResizeOp();
    virtual ~FSNResizeOp() override = default;

    virtual void OnDragged(const FDragDropEvent& DragDropEvent) override;
    virtual void OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent) override;

    static TSharedRef<FSNResizeOp> New(const TSharedRef<SSNNodeWidget>& InNodeWidget, const SSNNodeWidget::EDragType& InDragType,
                                           const FSimpleDelegate& InOnOperateDone);

private:
    TSharedPtr<SSNNodeWidget> NodeWidget;
    SSNNodeWidget::EDragType DragType;

    FSimpleDelegate OnOperateDone;
};
