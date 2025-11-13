// CrossableNodeDrawer.h
// @author : xiangyp
// @date : 2025/05/13 14:14:20
// @desc :

#pragma once
#include "CustomDrawer/SNCustomActionDrawerBase.h"

struct FSNNodeBlend;
class USNAnimationNode;

class FSNCrossableNodeDrawer : public FSNCustomActionDrawerBase
{
public:
    FSNCrossableNodeDrawer() = default;
    virtual ~FSNCrossableNodeDrawer() override = default;

    virtual void OnSlateRepaint(const USNAssetBase* Asset, const TSharedPtr<SOverlay>& Slot, const FSNSequencerContext* Context) override;
    virtual int OnWidgetPaint(const USNAssetBase* Asset, const FSNSequencerContext* Context, const FPaintArgs& Args,
                              const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
                              int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

protected:
    TArray<FVector2D> BlendInCurveValues;
    TArray<FVector2D> BlendOutCurveValues;

    void DrawCurve(const FSNNodeBlend& Blend, const FSNSequencerContext* Context, const FSlateLayoutTransform& Transform,
                   const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const;
};
