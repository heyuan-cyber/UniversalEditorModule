#include "SequencerEditor/SNTrackArea.h"

#include "SNHotkeys.h"
#include "SNSequencerContext.h"
#include "ScenarioNarrativeEditorDefines.h"
#include "SCurveEditor.h"
#include "Node/SNNodeBase.h"
#include "Preview/SNAssetPreviewCamera.h"
#include "Preview/SNPreviewManager.h"
#include "Preview/UI/SNAssetPreviewHUD.h"
#include "PreviewScene/SNAssetPreviewScene.h"
#include "Sequencer/SNSequencer.h"
#include "SequencerEditor/SNNodeWidget.h"
#include "SequencerEditor/SNTrackNodes.h"
#include "Widget/SNTimeRulerWidget.h"

void SSNTrackArea::Construct(const FArguments& InArgs)
{
    SetVisibility(EVisibility::SelfHitTestInvisible);
    Data = InArgs._Asset;
    Context = InArgs._Context;

    RemoveTrackHandle = Context->OnTrackDeleted.AddRaw(this, &SSNTrackArea::OnTrackChanged);
    CreateTrackHandle = Context->OnTrackCreated.AddRaw(this, &SSNTrackArea::OnTrackChanged);

    ChildSlot
    [
        SNew(SVerticalBox)
        + SVerticalBox::Slot()
        .HAlign(HAlign_Fill)
        .AutoHeight()
        [
            SAssignNew(TimeRuler, SSNTimeRulerWidget)
            .bShowScrubHandle_Lambda([this] { return Context->PreviewManager->State != ESNPreviewPlayerState::Stopped; })
            .Value_Lambda([this] { return Context->PreviewManager->CurrentTime; })
            .OnValueChanged_Lambda([this](const float& Val)
            {
                Context->PreviewManager->CurrentTime = FMath::Max(0, Val);
            })
            .Clipping(EWidgetClipping::ClipToBounds)
            .bAllowZoom(true)
            .SequenceLength(InArgs._SequenceLength)
            .NumOfKeys(InArgs._NumOfKeys)
            .ViewInputMin(InArgs._ViewInputMin)
            .ViewInputMax(InArgs._ViewInputMax)
            .OnSetInputViewRange(InArgs._OnSetInputViewRange)
            .OnMajorTickCreated_Lambda([this](const TArray<float>& Ticks)
            {
                MajorTicks = Ticks;
            })
        ]
        + SVerticalBox::Slot()
        .AutoHeight()
        [
            SNew(SSeparator)
        ]
        + SVerticalBox::Slot()
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Fill)
        [
            SAssignNew(Layout, SScrollBox)
            .Orientation(Orient_Vertical)
            .ScrollBarVisibility(EVisibility::Collapsed)
            .ScrollBarAlwaysVisible(false)
            .OnUserScrolled(InArgs._OnUserScrolled)
        ]
    ];

    Context->TimeRulerWidget = TimeRuler.Get();
    Context->TrackAreaWidget = this;

    BindCommands();
    Repaint();
}

SSNTrackArea::~SSNTrackArea()
{
    Context->OnTrackDeleted.Remove(RemoveTrackHandle);
    Context->OnTrackCreated.Remove(CreateTrackHandle);

    // UnbindCommands?
}

void SSNTrackArea::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    SCompoundWidget::OnMouseEnter(MyGeometry, MouseEvent);
    // refresh some command binding's SN
    BindCommands();
}

FReply SSNTrackArea::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    if (Layout->IsDirectlyHovered() && MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        Context->SetSelectTrack(nullptr);
        Context->SetSelectNode(nullptr);
    }

    if (TimeRuler)
    {
        return TimeRuler->OnMouseButtonDown(TimeRuler->GetCachedGeometry(), MouseEvent);
    }

    return SCompoundWidget::OnMouseButtonDown(MyGeometry, MouseEvent);
}

FReply SSNTrackArea::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    if (TimeRuler)
    {
        return TimeRuler->OnMouseButtonUp(TimeRuler->GetCachedGeometry(), MouseEvent);
    }

    return SCompoundWidget::OnMouseButtonUp(MyGeometry, MouseEvent);
}

FReply SSNTrackArea::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    if (TimeRuler)
    {
        return TimeRuler->OnMouseMove(TimeRuler->GetCachedGeometry(), MouseEvent);
    }

    return SCompoundWidget::OnMouseMove(MyGeometry, MouseEvent);
}

FReply SSNTrackArea::OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    if (TimeRuler && MouseEvent.IsControlDown())
    {
        return TimeRuler->OnMouseWheel(TimeRuler->GetCachedGeometry(), MouseEvent);
    }

    return SCompoundWidget::OnMouseWheel(MyGeometry, MouseEvent);
}

int32 SSNTrackArea::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
                                const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    int32 BaseLayer = SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
                                               bParentEnabled);
    const auto& TrackScaleInfo = TimeRuler->GetTrackScaleInfo();

    // draw length indicator
    const float Length = Data->GetMaxLength();
    if (Length > 0)
    {
        const float LengthPosX = TrackScaleInfo.InputToLocalX(Length);
        // const FVector2D Offset = FVector2D(0.f, TimeRuler->GetCachedGeometry().GetLocalSize().Y - 5.f);
        FSlateDrawElement::MakeBox
        (
            OutDrawElements,
            LayerId,
            AllottedGeometry.ToPaintGeometry(FVector2D(LengthPosX, TimeRuler->GetCachedGeometry().GetLocalSize().Y), {}),
            FAppStyle::GetBrush("Brushes.White"),
            ESlateDrawEffect::None,
            FColor(154, 229, 137, 100)
        );
    }

    // draw major tick line
    for (const float& TickPosX : MajorTicks)
    {
        DrawLine(TickPosX, AllottedGeometry, OutDrawElements, LayerId, false, InWidgetStyle.GetColorAndOpacityTint().CopyWithNewOpacity(0.1f));
    }

    // preview line
    if (Context->PreviewManager->State != ESNPreviewPlayerState::Stopped)
    {
        DrawLine(TrackScaleInfo.InputToLocalX(Context->PreviewManager->CurrentTime), AllottedGeometry, OutDrawElements, BaseLayer);
    }

    // magnetic line
    if (const auto& SelectedNode = Context->GetSelectNode())
    {
        const auto& Widget = Context->NodeToWidget[SelectedNode];
        if (Widget->CurrentDragType != SSNNodeWidget::EDragType::None)
        {
            float EqualTime = 0;
            if (Context->SetTimeIfNearlyEqualOther(SelectedNode, SelectedNode->BeginTime, &EqualTime))
            {
                DrawLine(TrackScaleInfo.InputToLocalX(EqualTime), AllottedGeometry, OutDrawElements, BaseLayer, true, FLinearColor::Yellow);
            }
            if (Context->SetTimeIfNearlyEqualOther(SelectedNode, SelectedNode->EndTime, &EqualTime))
            {
                DrawLine(TrackScaleInfo.InputToLocalX(EqualTime), AllottedGeometry, OutDrawElements, BaseLayer, true, FLinearColor::Yellow);
            }

            constexpr auto Tolerance = 0.001f;
            if (Context->PreviewManager->State != ESNPreviewPlayerState::Stopped &&
                (FMath::IsNearlyEqual(SelectedNode->BeginTime, Context->PreviewManager->CurrentTime, Tolerance) ||
                    FMath::IsNearlyEqual(SelectedNode->EndTime, Context->PreviewManager->CurrentTime, Tolerance)))
            {
                DrawLine(TrackScaleInfo.InputToLocalX(Context->PreviewManager->CurrentTime), AllottedGeometry, OutDrawElements, BaseLayer, false, FLinearColor::Yellow);
            }
        }
    }

    // total end line
    DrawLine(TrackScaleInfo.InputToLocalX(Data->GetMaxLength()), AllottedGeometry, OutDrawElements, BaseLayer, false, FColor::Magenta);

    return BaseLayer;
}

void SSNTrackArea::Repaint()
{
    Layout->ClearChildren();
    Context->TrackToNodesWidget.Empty();

    for (const auto& Track : Data->Tracks)
    {
        if (!Track)
        {
            Layout->AddSlot().Padding(FMargin(0, 2))[SNew(SBox).HeightOverride(40)
            [
                SNew(SBorder)
                [
                    SNew(STextBlock).Text(INVTEXT("ERROR TRACK!!!"))
                ]
            ]];
            continue;
        }

        TSharedPtr<SSNTrackNodes> TrackWidget;
        Layout->AddSlot().Padding(FMargin(0, 2))
        [
            SAssignNew(TrackWidget, SSNTrackNodes)
            .Clipping(EWidgetClipping::ClipToBounds)
            .TimeRuler(TimeRuler)
            .TrackData(Track)
            .Context(Context)
        ];

        Context->TrackToNodesWidget.Add(Track, TrackWidget.Get());
    }
}

void SSNTrackArea::SetScrollOffset(float Offset) const
{
    if (!Layout.IsValid() || Layout->GetScrollOffset() == Offset)
    {
        return;
    }

    Layout->SetScrollOffset(Offset);
}

void SSNTrackArea::OnTrackChanged(TObjectPtr<USNTrackBase> Track)
{
    Repaint();
}

void SSNTrackArea::DrawLine(const float& PosX, const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId,
                                bool bIsDashLine, FLinearColor LineColor) const
{
    if (PosX < 0)
    {
        return;
    }

    TArray<FVector2f> TickPos;
    TickPos.SetNumUninitialized(2);
    TickPos[0] = FVector2f(PosX, TimeRuler->GetCachedGeometry().GetLocalSize().Y);
    TickPos[1] = FVector2f(PosX, FMath::FloorToFloat(AllottedGeometry.GetLocalSize().Y));

    if (bIsDashLine)
    {
        FSlateDrawElement::MakeDashedLines
        (
            OutDrawElements,
            LayerId,
            AllottedGeometry.ToPaintGeometry(FVector2f::One(), {}),
            MoveTemp(TickPos),
            ESlateDrawEffect::None,
            LineColor
        );
    }
    else
    {
        FSlateDrawElement::MakeLines
        (
            OutDrawElements,
            LayerId,
            AllottedGeometry.ToPaintGeometry(FVector2D::One(), {}),
            TickPos,
            ESlateDrawEffect::None,
            LineColor,
            false
        );
    }
}

void SSNTrackArea::DeleteNode() const
{
    if (auto* OperateNode = SSNTrackNodes::CachedHoverNode == nullptr ? Context->GetSelectNode() : SSNTrackNodes::CachedHoverNode)
    {
        if (!Context->TrackToNodesWidget.Contains(OperateNode->Parent))
            return;

        Context->TrackToNodesWidget[OperateNode->Parent]->DeleteNode();
    }
}

void SSNTrackArea::BindCommands()
{
    Context->ToolkitCommands->MapAction(FSNHotkeys::Get().DeleteSNAsset, FExecuteAction::CreateSP(this, &SSNTrackArea::DeleteNode));
}
