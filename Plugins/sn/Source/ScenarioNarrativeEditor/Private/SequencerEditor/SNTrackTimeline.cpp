#include "SequencerEditor/SNTrackTimeline.h"

#include "SNSequencerContext.h"
#include "ScenarioNarrativeEditorDefines.h"
#include "Utils.h"
#include "EditorEngine/Setting/SNEditorSetting.h"
#include "Sequencer/SNSequencer.h"
#include "SequencerEditor/SNNodeWidget.h"
#include "SequencerEditor/SNTrackArea.h"
#include "SequencerEditor/SNTrackHierarchy.h"
#include "SequencerEditor/SNTrackNodes.h"
#include "SequencerEditor/SNTrackWidget.h"

void SSNTrackTimeline::Construct(const FArguments& InArgs)
{
    ViewMinVal = 0;
    ViewMaxVal = 1.f;
    SequenceLength = 1.f;

    Data = InArgs._Asset;
    Context = InArgs._Context;
    if (Data)
    {
        OnSetInputViewRange(0.f, FMath::Max(1.f, Data->GetMaxLength()));
    }

    ChildSlot
    [
        SNew(SOverlay)
        + SOverlay::Slot()
        [
            SNew(SSplitter)
            + SSplitter::Slot()
            .Resizable(true)
            .Value(0.2f)
            [
                SAssignNew(TrackHierarchy, SSNTrackHierarchy)
                .Asset(Data)
                .OnUserScrolled_Raw(this, &SSNTrackTimeline::OnTrackScroll)
                .Context(Context)
            ]
            + SSplitter::Slot()
            .Resizable(true)
            .Value(0.8f)
            [
                SAssignNew(TrackArea, SSNTrackArea)
                .Asset(Data)
                .SequenceLength_Raw(this, &SSNTrackTimeline::GetSequenceLength)
                .NumOfKeys_Raw(this, &SSNTrackTimeline::GetNOK)
                .ViewInputMin_Raw(this, &SSNTrackTimeline::GetViewMin)
                .ViewInputMax_Raw(this, &SSNTrackTimeline::GetViewMax)
                .OnSetInputViewRange_Raw(this, &SSNTrackTimeline::OnSetInputViewRange)
                .OnUserScrolled_Raw(this, &SSNTrackTimeline::OnTrackScroll)
                .Context(Context)
            ]
        ]
    ];
}

FReply SSNTrackTimeline::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    if (IsDirectlyHovered() && MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        Context->SetSelectNode(nullptr);
        return FReply::Handled();
    }

    return SCompoundWidget::OnMouseButtonDown(MyGeometry, MouseEvent);
}

FReply SSNTrackTimeline::OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    return SCompoundWidget::OnMouseWheel(MyGeometry, MouseEvent);
}

void SSNTrackTimeline::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
    SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

    const FVector2D& CursorPos = FSlateApplication::Get().GetCursorPos();
    UpdateHoverTrack(CursorPos);
    UpdateHoverNode(CursorPos);
}

void SSNTrackTimeline::OnSetInputViewRange(float InMinVal, float InMaxVal)
{
    ViewMinVal = FMath::Max(0, InMinVal);
    ViewMaxVal = FMath::Min(GetMaxKeyCounts() / (GetNOK() * EXTRA_LENGTH_RATE), InMaxVal);

    SequenceLength = FMath::Max(ViewMaxVal, SequenceLength);
    if (SequenceLength == ViewMaxVal)
    {
        SequenceLength *= EXTRA_LENGTH_RATE;
    }
}

uint32 SSNTrackTimeline::GetNOK() const
{
    USNEditorSetting* Settings = GetMutableDefault<USNEditorSetting>();
    return Settings->FrameRate;
}

uint32 SSNTrackTimeline::GetMaxKeyCounts() const
{
    USNEditorSetting* Settings = GetMutableDefault<USNEditorSetting>();
    return Settings->MAX_FRAME_COUNT;
}

void SSNTrackTimeline::OnTrackScroll(float Offset) const
{
    if (TrackHierarchy.IsValid())
    {
        TrackHierarchy->SetScrollOffset(Offset);
    }

    if (TrackArea.IsValid())
    {
        TrackArea->SetScrollOffset(Offset);
    }
}

void SSNTrackTimeline::UpdateHoverTrack(const FVector2D& CursorPos) const
{
    bool bIsHover = false;
    for (const auto& Pair : Context->TrackToWidget)
    {
        if (Pair.Value->GetCachedGeometry().IsUnderLocation(CursorPos))
        // if (Pair.Value->IsHovered())
        {
            Context->CurrentHoveredTrack = Pair.Key;
            bIsHover = true;
            break;
        }
    }

    if (!bIsHover)
    {
        for (const auto& Pair : Context->TrackToNodesWidget)
        {
            // if (Pair.Value->IsHovered())
            if (Pair.Value->GetCachedGeometry().IsUnderLocation(CursorPos))
            {
                Context->CurrentHoveredTrack = Pair.Key;
                bIsHover = true;
                break;
            }
        }
    }

    if (!bIsHover)
    {
        Context->CurrentHoveredTrack = nullptr;
    }
}

void SSNTrackTimeline::UpdateHoverNode(const FVector2D& CursorPos) const
{
    bool bIsHover = false;
    for (const auto& Pair : Context->NodeToWidget)
    {
        if (Pair.Value->IsHovered())
        // if (Pair.Value->GetCachedGeometry().IsUnderLocation(CursorPos))
        {
            Context->CurrentHoveredNode = Pair.Key;
            bIsHover = true;
            break;
        }
    }

    if (!bIsHover)
    {
        Context->CurrentHoveredNode = nullptr;
    }
}
