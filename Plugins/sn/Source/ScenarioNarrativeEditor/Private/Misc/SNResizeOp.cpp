#include "SNResizeOp.h"

#include "SNSequencerContext.h"
#include "ScenarioNarrativeEditorDefines.h"
#include "Widget/SNTimeRulerWidget.h"
#include "Utils.h"
#include "Node/SNNodeBase.h"

FSNResizeOp::FSNResizeOp()
{
    // dont create any decorator window
    bCreateNewWindow = false;
    DragType = SSNNodeWidget::EDragType::None;
}

void FSNResizeOp::OnDragged(const FDragDropEvent& DragDropEvent)
{
    FVector2D MousePos = DragDropEvent.GetScreenSpacePosition();
    float BeginPosX = MousePos.X;
    const auto& Node = NodeWidget->Data;

    const float MinBeginTime = FMath::Max(0, Node->EndTime - Node->GetMaxLength());
    const float MaxEndTime = Node->BeginTime + Node->GetMaxLength();
    const float MouseTime = DragDropEvent.IsShiftDown() ?
                                NodeWidget->Context->TimeRulerWidget->PositionToFrameTime(BeginPosX) :
                                NodeWidget->Context->TimeRulerWidget->PositionToTime(BeginPosX);
    const float EndTime = FMath::Min(MaxEndTime, MouseTime);

    switch (DragType)
    {
    case SSNNodeWidget::EDragType::ResizeLeft:
        Node->BeginTime = FMath::Clamp(MouseTime, MinBeginTime, Node->EndTime - Utils::ONE_FRAME_TIME);
        Node->bIsValid = !NodeWidget->Context->IsIntersectionWithOther(Node);
        Node->Modify();
        break;
    case SSNNodeWidget::EDragType::ResizeRight:
        Node->EndTime = FMath::Max(Node->BeginTime + Utils::ONE_FRAME_TIME, EndTime);
        Node->bIsValid = !NodeWidget->Context->IsIntersectionWithOther(Node);
        Node->Modify();
        break;
    default: break;
    }

    SetCursorOverride(EMouseCursor::Type::ResizeLeftRight);
}

void FSNResizeOp::OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent)
{
    // switch (DragType)
    // {
    // case SSNNodeWidget::EDragType::ResizeLeft:
    //     break;
    // case SSNNodeWidget::EDragType::ResizeRight:
    //     break;
    // default: break;
    // }

    OnOperateDone.ExecuteIfBound();
    SetCursorOverride(EMouseCursor::Type::Default);
}

TSharedRef<FSNResizeOp> FSNResizeOp::New(const TSharedRef<SSNNodeWidget>& InNodeWidget, const SSNNodeWidget::EDragType& InDragType,
                                                 const FSimpleDelegate& InOnOperateDone)
{
    TSharedRef<FSNResizeOp> Op = MakeShared<FSNResizeOp>();
    Op->NodeWidget = InNodeWidget;
    Op->DragType = InDragType;
    Op->OnOperateDone = InOnOperateDone;

    // we dont have decorator window, this function can skip
    Op->Construct();
    return Op;
}
