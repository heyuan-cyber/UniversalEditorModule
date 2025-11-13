// RecordInputNodeDrawer.h
// @author : xiangyp
// @date : 2025/06/09 17:48:54
// @desc :

#pragma once
#include "CustomDrawer/SNCustomActionDrawerBase.h"

class FSNRecordInputNodeDrawer : public FSNCustomActionDrawerBase
{
public:
    virtual void OnSlateRepaint(const USNAssetBase* Asset, const TSharedPtr<SOverlay>& Slot, const FSNSequencerContext* Context) override;
    virtual int OnWidgetPaint(const USNAssetBase* Asset, const FSNSequencerContext* Context, const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

private:
    FVector SliderPosition = FVector::Zero();
};

