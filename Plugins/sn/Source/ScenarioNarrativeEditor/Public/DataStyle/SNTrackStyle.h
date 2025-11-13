// SNTrackStyle.h
// @author : xiangyp
// @date : 2025/04/22 16:08:34
// @desc :

#pragma once
#include "SNStyleBase.h"

class USNTrackBase;

class FSNTrackStyle : public FSNAssetStyleBase
{
public:
    FSNTrackStyle();
    virtual ~FSNTrackStyle() override = default;

    virtual const FSlateColor& GetTrackEdColor() const { return TrackEdColor; }
    virtual const FSlateBrush* GetTrackIcon() const { return TrackIcon; }
    virtual float GetDesiredHeight() const { return DesiredHeight; }

    virtual void OnCreateContextMenu(FMenuBuilder& MenuBuilder, const USNAssetBase* Asset, FSNSequencerContext* Context) override final;

    virtual void OnContextMenu(FMenuBuilder& MenuBuilder, const USNAssetBase* Asset) {}

protected:
    FSlateColor TrackEdColor;
    const FSlateBrush* TrackIcon;
    float DesiredHeight;
};
