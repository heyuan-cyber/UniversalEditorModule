#include "SNGeneralDragDropOp.h"

FSNGeneralDragDropOp::FSNGeneralDragDropOp()
{
}

void FSNGeneralDragDropOp::OnDragged(const FDragDropEvent& DragDropEvent)
{
    DraggedOp.ExecuteIfBound(DragDropEvent);
}

void FSNGeneralDragDropOp::OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent)
{
    DropOp.ExecuteIfBound(bDropWasHandled, MouseEvent);
}

TSharedRef<FSNGeneralDragDropOp> FSNGeneralDragDropOp::New(const FOnDraggedCallback& DraggedCallback, const FOnDropCallback& DropCallback)
{
    TSharedRef<FSNGeneralDragDropOp> Op = MakeShared<FSNGeneralDragDropOp>();
    Op->DraggedOp = DraggedCallback;
    Op->DropOp = DropCallback;
    return Op;
}
