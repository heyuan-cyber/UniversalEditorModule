// GeneralDragDropOp.h
// @author : xiangyp
// @date : 2025/06/10 11:03:36
// @desc : 通用drag/drop操作

#pragma once

DECLARE_DELEGATE_OneParam(FOnDraggedCallback, const class FDragDropEvent&);
DECLARE_DELEGATE_TwoParams(FOnDropCallback, bool, const FPointerEvent&);

class FSNGeneralDragDropOp : public FDragDropOperation
{
public:
    DRAG_DROP_OPERATOR_TYPE(FGenralDragDropOp, FDragDropOperation)

    FSNGeneralDragDropOp();
    virtual ~FSNGeneralDragDropOp() override = default;

    virtual void OnDragged(const FDragDropEvent& DragDropEvent) override;
    virtual void OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent) override;

    static TSharedRef<FSNGeneralDragDropOp> New(const FOnDraggedCallback& DraggedCallback, const FOnDropCallback& DropCallback);

private:
    FOnDraggedCallback DraggedOp;
    FOnDropCallback DropOp;
};
