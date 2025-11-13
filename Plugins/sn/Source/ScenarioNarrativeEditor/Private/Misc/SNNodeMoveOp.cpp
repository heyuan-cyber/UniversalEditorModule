#include "SNNodeMoveOp.h"

#include "SNSequencerContext.h"
#include "ScenarioNarrativeEditorDefines.h"
#include "Widget/SNTimeRulerWidget.h"
#include "Node/SNNodeBase.h"
#include "Node/SNCrossableNode.h"
#include "SequencerEditor/SNNodeWidget.h"
#include "SequencerEditor/SNTrackNodes.h"
#include "Track/SNTrackBase.h"

FSNNodeMoveOp::FSNNodeMoveOp(): RawTime()
{
    // we dont need any decorator window
    bCreateNewWindow = false;
}

void FSNNodeMoveOp::OnDragged(const FDragDropEvent& DragDropEvent)
{
    if (!NodeWidget.IsValid() || !NodeWidget->Context->TimeRulerWidget)
    {
        return;
    }

    FVector2D MousePos = DragDropEvent.GetScreenSpacePosition();
    float BeginPosX = MousePos.X - Offset;

    const auto& Node = NodeWidget->Data;
    float Length = FMath::Max(Node->GetMinLength(), Node->Length());

    const auto* TimeRulerWidget = NodeWidget->Context->TimeRulerWidget;
    NodeWidget->Data->BeginTime = DragDropEvent.IsShiftDown()
                                      ? TimeRulerWidget->PositionToFrameTime(BeginPosX)
                                      : TimeRulerWidget->PositionToTime(BeginPosX);
    Node->EndTime = Node->BeginTime + Length;

    const auto& TargetTrack = NodeWidget->Context->CurrentHoveredTrack;
    if (TargetTrack && TargetTrack != LastSavedTrack && Node->CanAttachTo(TargetTrack->GetClass()))
    {
        // LastSavedTrack->RemoveNode(Node);
        TargetTrack->AddNode(Node);

        (*NodeWidget->Context->TrackToNodesWidget.Find(LastSavedTrack))->Repaint();
        (*NodeWidget->Context->TrackToNodesWidget.Find(TargetTrack))->Repaint();
        LastSavedTrack = TargetTrack;
    }

    if (!Node->IsA(USNCrossableNode::StaticClass()))
    {
        Node->bIsValid = !NodeWidget->Context->IsIntersectionWithOther(Node);
    }

    Node->Modify();

    SetCursorOverride(EMouseCursor::Type::GrabHand);
}

void FSNNodeMoveOp::OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent)
{
    if (!NodeWidget.IsValid() || !NodeWidget->Data)
    {
        UE_LOG(LogScenarioNarrativeEditor, Error, TEXT("some thing was wrong."));
        return;
    }

    const auto& Data = NodeWidget->Data;
    // if not valid, recover saved data
    if (!Data->bIsValid)
    {
        Data->BeginTime = RawTime.X;
        Data->EndTime = RawTime.Y;

        RawTrack->AddNode(Data);
        Data->bIsValid = true;

        (*NodeWidget->Context->TrackToNodesWidget.Find(LastSavedTrack))->Repaint();
        (*NodeWidget->Context->TrackToNodesWidget.Find(RawTrack))->Repaint();
        Data->Modify();
    }

    OnOperateDone.ExecuteIfBound();
    SetCursorOverride(EMouseCursor::Type::Default);
}

TSharedRef<FSNNodeMoveOp> FSNNodeMoveOp::New(const TSharedRef<SSNNodeWidget>& InNodeWidget, float InOffset,
                                                     const TObjectPtr<USNTrackBase>& InTrackData, const FSimpleDelegate& InOperateDone)
{
    TSharedRef<FSNNodeMoveOp> Op = MakeShared<FSNNodeMoveOp>();
    Op->NodeWidget = InNodeWidget;
    Op->Offset = InOffset;
    Op->RawTime = {InNodeWidget->Data->BeginTime, InNodeWidget->Data->EndTime};
    Op->RawTrack = InTrackData;
    Op->LastSavedTrack = InTrackData;
    Op->OnOperateDone = InOperateDone;
    Op->Construct();

    return Op;
}
