#pragma once
#include "CustomDrawer/SNCustomActionDrawerBase.h"

class USNNodeBase;
/**
 * 通用的关键帧绘制类，用于定义关键帧的样式
 */
class SCENARIONARRATIVEEDITOR_API FSNKeyFrameDrawer : public FSNCustomActionDrawerBase
{
private:
    FVector SliderPosition = FVector::Zero();

public:
    virtual void OnSlateRepaint(const USNAssetBase* Asset, const TSharedPtr<SOverlay>& Slot, const FSNSequencerContext* Context) override;

    virtual int OnWidgetPaint(const USNAssetBase* Asset, const FSNSequencerContext* Context, const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

};
