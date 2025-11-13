#include "SNTrackMoveOp.h"

#include "SNSequencerContext.h"
#include "ScenarioNarrativeEditorDefines.h"
#include "Utils.h"

FSNTrackMoveOp::FSNTrackMoveOp()
{
}

void FSNTrackMoveOp::OnDragged(const FDragDropEvent& DragDropEvent)
{
    Context->SetTrackIsDragging(Context->TrackToWidget[DragTrack]);
    SetCursorOverride(EMouseCursor::Type::GrabHand);
}

void FSNTrackMoveOp::OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent)
{
    Context->ClearTrackDragging();
    SetCursorOverride(EMouseCursor::Type::Default);

    if (!Context->CurrentHoveredTrack ||
        !Context->TrackToWidget.Contains(Context->CurrentHoveredTrack))
        return;

    // const auto* Widget = Context->TrackToWidget[Context->CurrentHoveredTrack];
    // if (Widget == DragTrackWidget)
    //     return;
    if (DragTrack == Context->CurrentHoveredTrack)
        return;

    const bool bIsTop = Utils::IsTopOfWidget(Context->TrackToWidget[Context->CurrentHoveredTrack], MouseEvent.GetScreenSpacePosition().Y);
    Context->MoveTrack(DragTrack, Context->CurrentHoveredTrack, !bIsTop);

    OnOperateDone.ExecuteIfBound();
}

TSharedRef<FSNTrackMoveOp> FSNTrackMoveOp::New(USNTrackBase* InTrack, FSNSequencerContext* InContext, const FSimpleDelegate& InOperateDone)
{
    TSharedRef<FSNTrackMoveOp> Op = MakeShared<FSNTrackMoveOp>();
    Op->DragTrack = InTrack;
    Op->Context = InContext;
    Op->OnOperateDone = InOperateDone;

    return Op;
}
