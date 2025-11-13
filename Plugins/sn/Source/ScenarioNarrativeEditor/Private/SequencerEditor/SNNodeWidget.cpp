#include "SequencerEditor/SNNodeWidget.h"

#include "SNSequencerContext.h"
#include "ScenarioNarrativeEditorDefines.h"
#include "CustomDrawer/SNCustomActionDrawerBase.h"
#include "CustomDrawer/SNCustomActionDrawerRegistry.h"
#include "DataStyle/SNNodeStyle.h"
#include "DataStyle/SNAssetStyleRegistry.h"
#include "Misc/SNNodeMoveOp.h"
#include "Misc/SNResizeOp.h"
#include "Node/SNNodeBase.h"
#include "Preview/SNPreviewManager.h"
#include "Widget/SNHoverableBox.h"

#define LOCTEXT_NAMESPACE "SSNNodeWidget"

const float SSNNodeWidget::FLAG_COLOR_HEIGHT = 4.f;
const float SSNNodeWidget::MAX_RESIZE_BUTTON_WIDTH = 10.f;

void SSNNodeWidget::Construct(const FArguments& InArgs)
{
    Data = InArgs._NodeData;
    Context = InArgs._Context;
    ChildSlot
    [
        SAssignNew(Slot, SOverlay)
    ];

    Repaint();
}

FReply SSNNodeWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    if (IsHovered() && MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        Context->SetSelectNode(Data);
        return DetectDrag(EDragType::Move);
    }

    return SCompoundWidget::OnMouseButtonDown(MyGeometry, MouseEvent);
}

FReply SSNNodeWidget::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        CurrentDragType = EDragType::None;
        return FReply::Handled().ReleaseMouseCapture();
    }

    return SCompoundWidget::OnMouseButtonUp(MyGeometry, MouseEvent);
}

FReply SSNNodeWidget::OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
    {
        const float Offset = MouseEvent.GetScreenSpacePosition().X - MyGeometry.GetAbsolutePosition().X;
        switch (CurrentDragType)
        {
        case EDragType::Move:
            return FReply::Handled().BeginDragDrop(FSNNodeMoveOp::New(SharedThis(this), Offset, Context->CurrentHoveredTrack,
                                                                          FSimpleDelegate::CreateRaw(this, &SSNNodeWidget::OnOperateDone)));
        case EDragType::ResizeLeft:
        case EDragType::ResizeRight:
            return FReply::Handled().BeginDragDrop(FSNResizeOp::New(SharedThis(this), CurrentDragType,
                                                                        FSimpleDelegate::CreateRaw(this, &SSNNodeWidget::OnOperateDone)));
        default:
            break;
        }
    }

    return SCompoundWidget::OnDragDetected(MyGeometry, MouseEvent);
}

int32 SSNNodeWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                 const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
                                 const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    int BaseLayerId = SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
    if (const auto& Drawer = FSNCustomActionDrawerRegistry::Get().GetDrawer(Data))
    {
        BaseLayerId = FMath::Max(BaseLayerId, Drawer->OnWidgetPaint(Data, Context.Get(), Args, AllottedGeometry, MyCullingRect, OutDrawElements,
                                                                    LayerId, InWidgetStyle, bParentEnabled));
    }

    if (!Data->Validate())
    {
        FSlateDrawElement::MakeBox
        (
            OutDrawElements,
            LayerId + 1,
            AllottedGeometry.ToPaintGeometry(),
            FAppStyle::GetBrush("Brushes.White"),
            ESlateDrawEffect::None,
            FLinearColor::Red.CopyWithNewOpacity(0.6f)
        );
    }

    return BaseLayerId;
}

void SSNNodeWidget::Repaint()
{
    if (!Data)
    {
        Slot->AddSlot()
        [
            SNew(SBorder)
            .Visibility(EVisibility::HitTestInvisible)
            .BorderImage(FAppStyle::GetBrush("Brushes.White"))
            .BorderBackgroundColor(FColor(158, 41, 39))
            [
                SNew(STextBlock)
                .Text(INVTEXT("ERROR NODE"))
            ]
        ];
        return;
    }

    const auto* Style = FSNAssetStyleRegistry::Get().GetNodeStyle(Data->GetClass());
    auto* Drawer = FSNCustomActionDrawerRegistry::Get().GetDrawer(Data);

    Slot->ClearChildren();
    if (!Drawer || Drawer->IsDrawDefaultStyle())
    {
        Slot->AddSlot()
        [
            SNew(SBorder)
            .Visibility(EVisibility::HitTestInvisible)
            .BorderImage(FAppStyle::GetBrush("Brushes.White"))
            .BorderBackgroundColor_Lambda([Style] { return Style->GetBackgroundColor(); })
        ];
    }

    if (Drawer)
    {
        Drawer->OnSlateRepaint(Data, Slot, Context.Get());
    }

    if (!Drawer || Drawer->IsDrawDefaultStyle())
    {
        Slot->AddSlot()
        [
            SNew(SVerticalBox)
            + SVerticalBox::Slot()
            .HAlign(HAlign_Center)
            .VAlign(VAlign_Center)

            .FillHeight(1.f)
            [
                SNew(STextBlock)
                .Text_Lambda([this] { return FText::FromName(Data->DisplayName); })
                .TextStyle(FAppStyle::Get(), "LargeText")
                .ShadowColorAndOpacity(FLinearColor::Black.CopyWithNewOpacity(0.3f))
                .ShadowOffset(FVector2D::UnitVector)
                .Justification(ETextJustify::Type::Center)
                .Visibility(EVisibility::HitTestInvisible)
            ]
            + SVerticalBox::Slot()
            .AutoHeight()
            [
                SNew(SBox)
                .HeightOverride(FLAG_COLOR_HEIGHT)
                [
                    SNew(SBorder)
                    .BorderImage(FAppStyle::GetBrush("Brushes.White"))
                    .BorderBackgroundColor_Lambda([Style] { return Style->GetFlagColor(); })
                ]
            ]
        ];
    }

    // draw resize handle
    Slot->AddSlot()
    [
        SNew(SHorizontalBox)
        .Visibility_Lambda([this] { return IsHovered() && Data->bResizable ? EVisibility::SelfHitTestInvisible : EVisibility::Collapsed; })
        + SHorizontalBox::Slot()
        .HAlign(HAlign_Left)
        [
            SNew(SBox)
            .Clipping(EWidgetClipping::ClipToBounds)
            .WidthOverride(MAX_RESIZE_BUTTON_WIDTH)
            [
                SNew(SBorder)
                .OnMouseButtonDown(FPointerEventHandler::CreateSP(this, &SSNNodeWidget::DetectDrag, EDragType::ResizeLeft))
                .OnMouseButtonUp(FPointerEventHandler::CreateSP(this, &SSNNodeWidget::DetectDrag, EDragType::None))
                .BorderImage(FAppStyle::GetBrush("Brushes.White"))
                .BorderBackgroundColor_Lambda([Style] { return Style->GetFlagColor(); })
                [
                    SNew(SSNHoverableBox)
                    .OnMouseEnter_Lambda([this]{ SetCursor(EMouseCursor::ResizeLeftRight); })
                    .OnMouseExit_Lambda([this] { SetCursor(EMouseCursor::Default); })
                ]
            ]
        ]
        + SHorizontalBox::Slot()
        .HAlign(HAlign_Right)
        [
            SNew(SBox)
            .Clipping(EWidgetClipping::ClipToBounds)
            .WidthOverride(MAX_RESIZE_BUTTON_WIDTH)
            [
                SNew(SBorder)
                .OnMouseButtonDown(FPointerEventHandler::CreateSP(this, &SSNNodeWidget::DetectDrag, EDragType::ResizeRight))
                .OnMouseButtonUp(FPointerEventHandler::CreateSP(this, &SSNNodeWidget::DetectDrag, EDragType::None))
                .BorderImage(FAppStyle::GetBrush("Brushes.White"))
                .BorderBackgroundColor_Lambda([Style] { return Style->GetFlagColor(); })
                [
                    SNew(SSNHoverableBox)
                    .OnMouseEnter_Lambda([this]{ SetCursor(EMouseCursor::ResizeLeftRight); })
                    .OnMouseExit_Lambda([this] { SetCursor(EMouseCursor::Default); })
                ]
            ]
        ]
    ];

    Slot->AddSlot()
    [
        SNew(SBorder)
        .BorderImage(FAppStyle::GetBrush("MarqueeSelection"))
        .BorderBackgroundColor_Lambda([this]
        {
            if (Context->GetSelectNode() == Data)
            {
                return FLinearColor::Green;
            }
            if (Context->CurrentHoveredNode == Data)
            {
                return FLinearColor::White;
            }

            return FLinearColor::White;
        })
        .Visibility_Lambda([this]
        {
            return (Context->CurrentHoveredNode == Data || Context->GetSelectNode() == Data)
                       ? EVisibility::SelfHitTestInvisible
                       : EVisibility::Collapsed;
        })
    ];

    SetToolTipText(TAttribute<FText>::CreateLambda([this, Style]
    {
        FText Fmt = Style->GetTypeName().IsEmpty() ? INVTEXT("[{0}{1}]") : INVTEXT("[{0} - {1}]");
        FText TooltipPrefix = FText::Format(Fmt, FText::FromString(Data->GetClass()->GetName()), FText::FromString(Style->GetTypeName()));
        return Style->GetTooltipText(Data).IsEmpty()
                   ? TooltipPrefix
                   : FText::Format(INVTEXT("[{0}]\n{1}"), {TooltipPrefix, Style->GetTooltipText(Data)});
    }));
}

void SSNNodeWidget::OnOperateDone()
{
    CurrentDragType = EDragType::None;

    float EqualTime = 0;
    if (Context->SetTimeIfNearlyEqualOther(Data, Data->BeginTime, &EqualTime))
    {
        Data->BeginTime = EqualTime;
    }
    else if (Context->SetTimeIfNearlyEqualOther(Data, Data->EndTime, &EqualTime))
    {
        Data->EndTime = EqualTime;
    }
    else if (Context->PreviewManager->State != ESNPreviewPlayerState::Stopped)
    {
        if (FMath::IsNearlyEqual(Data->BeginTime, Context->PreviewManager->CurrentTime, 0.001f))
            Data->BeginTime = Context->PreviewManager->CurrentTime;
        else if (FMath::IsNearlyEqual(Data->EndTime, Context->PreviewManager->CurrentTime, 0.001f))
            Data->EndTime = Context->PreviewManager->CurrentTime;
    }
}

FReply SSNNodeWidget::DetectDrag(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent, EDragType DragType)
{
    CurrentDragType = DragType;
    if (CurrentDragType != EDragType::ResizeLeft && CurrentDragType != EDragType::ResizeRight)
    {
        return FReply::Handled().ReleaseMouseCapture();
    }

    return FReply::Handled().DetectDrag(AsShared(), EKeys::LeftMouseButton);
}

#undef LOCTEXT_NAMESPACE
