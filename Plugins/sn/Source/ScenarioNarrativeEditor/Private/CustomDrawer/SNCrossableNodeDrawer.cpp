#include "SNCrossableNodeDrawer.h"
#include "Node/SNCrossableNode.h"
#include "SNSequencerContext.h"
#include "SequencerEditor/SNNodeWidget.h"
#include "Widget/SNTimeRulerWidget.h"

void FSNCrossableNodeDrawer::OnSlateRepaint(const USNAssetBase* Asset, const TSharedPtr<SOverlay>& Slot, const FSNSequencerContext* Context)
{
}

int FSNCrossableNodeDrawer::OnWidgetPaint(const USNAssetBase* Asset, const FSNSequencerContext* Context, const FPaintArgs& Args,
                                        const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
                                        int32 LayerId,
                                        const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    if (!Asset->Validate())
    {
        return LayerId;
    }

    const USNCrossableNode* Node = Cast<USNCrossableNode>(Asset);

    DrawCurve(Node->BlendIn, Context, {}, AllottedGeometry, OutDrawElements, LayerId);

    const float Width = Context->TimeRulerWidget->TimeToScaledLength(Node->BlendOut.BlendTime);
    const FVector2f Offset = {AllottedGeometry.GetLocalSize().X - Width, 0};
    DrawCurve(Node->BlendOut, Context, FSlateLayoutTransform(Offset), AllottedGeometry, OutDrawElements, LayerId);

    return LayerId + 1;
}

void FSNCrossableNodeDrawer::DrawCurve(const FSNNodeBlend& Blend, const FSNSequencerContext* Context, const FSlateLayoutTransform& Transform,
                                     const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const
{
    {
        if (Blend.BlendTime <= 0)
        {
            return;
        }

        const float Width = Context->TimeRulerWidget->TimeToScaledLength(Blend.BlendTime);
        const FVector2D GeoSize = {Width, AllottedGeometry.GetLocalSize().Y - SSNNodeWidget::FLAG_COLOR_HEIGHT};
        const FPaintGeometry Geometry = AllottedGeometry.ToPaintGeometry(GeoSize, Transform);

        TArray<FVector2D> Points;
        float Alpha = 0;
        while (Alpha <= Blend.BlendTime)
        {
            float X = Alpha * Width / Blend.BlendTime;
            float Y = Geometry.GetLocalSize().Y - Blend.GetBlendValue(Alpha) * Geometry.GetLocalSize().Y;
            Points.Add({X, Y});
            Alpha += 0.01f;
        }
        Points.Add({Width, Geometry.GetLocalSize().Y - Blend.GetBlendValue(Blend.BlendTime) * Geometry.GetLocalSize().Y});

        FSlateDrawElement::MakeLines
        (
            OutDrawElements,
            LayerId + 2,
            Geometry,
            Points,
            ESlateDrawEffect::None,
            FLinearColor::Yellow,
            true,
            2.f
        );
    }
}
