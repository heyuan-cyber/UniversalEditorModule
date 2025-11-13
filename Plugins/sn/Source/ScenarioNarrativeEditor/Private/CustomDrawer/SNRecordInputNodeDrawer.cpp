#include "SNRecordInputNodeDrawer.h"

#include "SNSequencerContext.h"
#include "SCurveEditor.h"
#include "Misc/SNGeneralDragDropOp.h"
#include "Node/SNRecordInputNode.h"
#include "Widget/SNTimeRulerWidget.h"

void FSNRecordInputNodeDrawer::OnSlateRepaint(const USNAssetBase* Asset, const TSharedPtr<SOverlay>& Slot, const FSNSequencerContext* Context)
{
    Slot->AddSlot()
    [
        SNew(SBox)
        .Padding_Lambda([Asset, Slot]
        {
            constexpr float HalfWidth = 3;
            const USNRecordInputNode* Node = Cast<USNRecordInputNode>(Asset);
            const float Left = Slot->GetCachedGeometry().Size.X * Node->DisableTime / Node->Length();
            const float Right = Slot->GetCachedGeometry().Size.X - Left - HalfWidth;
            return FMargin(Left - HalfWidth, 0, Right, 0);
        })
        [
            SNew(SBorder)
            .Padding(200.f)
            .BorderImage(FAppStyle::GetBrush("Brushes.White"))
            .BorderBackgroundColor(FColor::Silver)
            .OnMouseButtonDown_Lambda([Asset, Context](const FGeometry&, const FPointerEvent&)
            {
                FSlateApplication::Get().GetPlatformCursor()->SetType(EMouseCursor::Type::ResizeLeftRight);
                return FReply::Handled().BeginDragDrop(FSNGeneralDragDropOp::New(FOnDraggedCallback::CreateLambda([Asset, Context](const FDragDropEvent& Evt)
                {
                    auto ScaleInfo = Context->TimeRulerWidget->GetTrackScaleInfo();
                    USNRecordInputNode* Node = Cast<USNRecordInputNode>(const_cast<USNAssetBase*>(Asset));
                    float BeginX = ScaleInfo.InputToLocalX(Node->BeginTime);
                    float MouseTime = Evt.IsShiftDown() ?
                        Context->TimeRulerWidget->PositionToFrameTime(Evt.GetScreenSpacePosition().X - BeginX) :
                        Context->TimeRulerWidget->PositionToTime(Evt.GetScreenSpacePosition().X - BeginX);

                    Node->DisableTime = FMath::Clamp(MouseTime, 0, Node->Length());
                }), nullptr));
            })
            .OnMouseButtonUp_Lambda([](const FGeometry&, const FPointerEvent&)
            {
                FSlateApplication::Get().GetPlatformCursor()->SetType(EMouseCursor::Type::Default);
                return FReply::Handled().ReleaseMouseCapture();
            })
        ]
    ];
}

int FSNRecordInputNodeDrawer::OnWidgetPaint(const USNAssetBase* Asset, const FSNSequencerContext* Context, const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    if (!Asset->Validate())
        return LayerId;

    const USNRecordInputNode* Node = Cast<USNRecordInputNode>(Asset);
    const float X = AllottedGeometry.Size.X * Node->DisableTime / Node->Length();
    const FSlateLayoutTransform Transform({X, 0});
    // Node->DisableTime

    FSlateDrawElement::MakeBox
    (
        OutDrawElements,
        ++LayerId,
        AllottedGeometry.ToPaintGeometry({X, AllottedGeometry.Size.Y}, {}),
        FAppStyle::Get().GetBrush("Brushes.White"),
        ESlateDrawEffect::None,
        FColor(127, 255, 255)
    );

    FSlateDrawElement::MakeBox
    (
        OutDrawElements,
        LayerId,
        AllottedGeometry.ToPaintGeometry({AllottedGeometry.Size.X - X, AllottedGeometry.Size.Y}, Transform),
        FAppStyle::Get().GetBrush("Brushes.White"),
        ESlateDrawEffect::None,
        FColor(250, 250, 120)
    );

    return LayerId;
}
