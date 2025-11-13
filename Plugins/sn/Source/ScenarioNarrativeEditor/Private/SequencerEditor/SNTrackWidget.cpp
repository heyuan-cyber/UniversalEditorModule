#include "SequencerEditor/SNTrackWidget.h"

#include "SNSequencerContext.h"
#include "ScenarioNarrativeEditorDefines.h"
#include "Utils.h"
#include "CustomDrawer/SNCustomActionDrawerBase.h"
#include "CustomDrawer/SNCustomActionDrawerRegistry.h"
#include "DataStyle/SNAssetStyleRegistry.h"
#include "DataStyle/SNTrackStyle.h"
#include "Misc/SNTrackMoveOp.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Track/SNTrackBase.h"

#define LOCTEXT_NAMESPACE "SNTrackWidgetBase"

void SSNTrackWidget::Construct(const FArguments& InArgs)
{
    Data = InArgs._TrackData;
    Context = InArgs._Context;
    ChildSlot[SAssignNew(Slot, SOverlay)];
    Repaint();
}

FReply SSNTrackWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    if (IsHovered() && MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        Context->SetSelectTrack(Data);
        return FReply::Handled();
    }

    return SCompoundWidget::OnMouseButtonDown(MyGeometry, MouseEvent);
}

int32 SSNTrackWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                  const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
                                  const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    int BaseLayerId = SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

    if (bIsDrop)
    {
        float ScreenPosY = FSlateApplication::Get().GetCursorPos().Y - AllottedGeometry.GetAbsolutePosition().Y;
        if (ScreenPosY >= 0)
        {
            if (Utils::IsTopOfWidget(this, FSlateApplication::Get().GetCursorPos().Y))
            {
                const FPaintGeometry Geometry = AllottedGeometry.ToPaintGeometry({ AllottedGeometry.GetLocalSize().X, 3.f}, {});
                FSlateDrawElement::MakeBox
                (
                    OutDrawElements,
                    BaseLayerId,
                    Geometry,
                    FAppStyle::Get().GetBrush("Brushes.Dropdown")
                );
            }
            else
            {
                const FSlateLayoutTransform Transform({0, AllottedGeometry.GetLocalSize().Y - 3.f});
                const FPaintGeometry Geometry = AllottedGeometry.ToPaintGeometry({ AllottedGeometry.GetLocalSize().X, 3.f}, Transform);
                FSlateDrawElement::MakeBox
                (
                    OutDrawElements,
                    BaseLayerId,
                    Geometry,
                    FAppStyle::Get().GetBrush("Brushes.Dropdown")
                );
            }
        }
    }

    if (const auto& Drawer = FSNCustomActionDrawerRegistry::Get().GetDrawer(Data))
    {
        BaseLayerId = FMath::Max(BaseLayerId, Drawer->OnWidgetPaint(Data, Context.Get(), Args, AllottedGeometry, MyCullingRect, OutDrawElements,
                                                                    LayerId, InWidgetStyle, bParentEnabled));
    }

    return BaseLayerId;
}

FReply SSNTrackWidget::OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
    {
        return FReply::Handled().BeginDragDrop(FSNTrackMoveOp::New(Data, Context.Get(), nullptr));
    }

    return SCompoundWidget::OnDragDetected(MyGeometry, MouseEvent);
}

void SSNTrackWidget::Repaint()
{
    Slot->ClearChildren();

    Slot->AddSlot()
    [
        SNew(SBorder)
        .Visibility(EVisibility::HitTestInvisible)
        .BorderImage_Lambda([this]
        {
            if (Context->GetSelectTrack() == Data || Context->CurrentHoveredTrack == Data)
            {
                return FAppStyle::GetBrush("Brushes.Hover");
            }

            return FAppStyle::GetBrush("Brushes.Panel");
        })
    ];

    const FSNTrackStyle* Style = FSNAssetStyleRegistry::Get().GetTrackStyle(Data->GetClass());
    auto* Drawer = FSNCustomActionDrawerRegistry::Get().GetDrawer(Data);

    TSharedPtr<SHorizontalBox> TrackHBox = nullptr;
    Slot->AddSlot()
    [
        SNew(SBox)
        .HeightOverride(Style->GetDesiredHeight())
        [
            SAssignNew(TrackHBox, SHorizontalBox)
        ]
    ];

    constexpr float DragIconWidth = 15.f;
    TrackHBox->AddSlot().MaxWidth(DragIconWidth).MinWidth(DragIconWidth)
    [
        SNew(SScaleBox)
        .Visibility_Lambda([this] { return IsHovered() ? EVisibility::Visible : EVisibility::Hidden; })
        .HAlign(HAlign_Center)
        .Stretch(EStretch::ScaleToFit)
        [
            SNew(SImage)
            .Image(FAppStyle::Get().GetBrush("Icons.DragHandle"))
            .OnMouseButtonDown(FPointerEventHandler::CreateLambda([this](const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
            {
                return FReply::Handled().DetectDrag(AsShared(), EKeys::LeftMouseButton);
            }))
        ]
    ];

    if (!Drawer || Drawer->IsDrawDefaultStyle())
    {
        if (Style->GetTrackIcon())
        {
            TrackHBox->AddSlot().AutoWidth().Padding(FMargin(0, 5, 5, 5))
            [
                SNew(SBox)
                .WidthOverride(50)
                [
                    SNew(SScaleBox)
                    .Stretch(EStretch::ScaleToFit)
                    [
                        SNew(SImage)
                        .Image(Style->GetTrackIcon())
                    ]
                ]
            ];
        }

        const FString Typename = Style->GetTypeName().IsEmpty() ? Data->GetClass()->GetName() : Style->GetTypeName();

        TSharedPtr<SVerticalBox> NameVBox = nullptr;
        SAssignNew(NameVBox, SVerticalBox);
        NameVBox->AddSlot().VAlign(VAlign_Center).FillHeight(1.f)
        [
            SNew(STextBlock)
            .Text_Lambda([this] { return FText::FromName(Data->DisplayName); })
            .Justification(ETextJustify::Type::Left)
        ];

        TrackHBox->AddSlot().FillContentWidth(1.f)[NameVBox.ToSharedRef()];
        if (Style->GetDesiredHeight() > 30.f)
        {
            NameVBox->AddSlot().FillHeight(0.35f).VAlign(VAlign_Center)
            [
                SNew(STextBlock)
                .TextStyle(FAppStyle::Get(), TEXT("SmallText"))
                .Text(FText::FromString(Typename))
                .Justification(ETextJustify::Type::Left)
            ];
        }
        else
        {
            TrackHBox->AddSlot().AutoWidth().VAlign(VAlign_Center).Padding(FMargin(0, 0, 5, 0))
            [
                SNew(STextBlock)
                .TextStyle(FAppStyle::Get(), TEXT("SmallText"))
                .Text(FText::FromString(Typename))
                .Justification(ETextJustify::Type::Left)
            ];
        }

        if (Style->GetTrackEdColor() != FColor::Transparent)
        {
            TrackHBox->AddSlot().AutoWidth()
            [
                SNew(SBox)
                .WidthOverride(4)
                [
                    SNew(SBorder)
                    .BorderImage(FAppStyle::GetBrush("Brushes.White"))
                    .BorderBackgroundColor(Style->GetTrackEdColor())
                ]
            ];
        }
    }

    if (Drawer)
    {
        Drawer->OnSlateRepaint(Data, Slot, Context.Get());
    }

    Slot->SetToolTipText(TAttribute<FText>::CreateLambda([this, Style] { return Style->GetTooltipText(Data); }));
}

#undef LOCTEXT_NAMESPACE
