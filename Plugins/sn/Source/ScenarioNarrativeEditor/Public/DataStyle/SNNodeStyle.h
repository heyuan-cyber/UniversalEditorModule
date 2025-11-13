#pragma once
#include "SNSequencerContext.h"
#include "SNStyleBase.h"

class USNNodeBase;

class FSNNodeStyle : public FSNAssetStyleBase
{
public:
    FSNNodeStyle();
    virtual ~FSNNodeStyle() override = default;

    virtual const FSlateColor& GetBackgroundColor() const { return BackgroundColor; }
    virtual const FSlateColor& GetFlagColor() const { return BottomColor; }
    virtual const FSlateIcon& GetIcon() const { return Icon; }
    virtual FString GetTypeName() const override { return TypeName; };
    virtual FText GetTooltipText(const TObjectPtr<USNAssetBase>& Asset) const override { return TooltipText; };
    virtual void OnCreateContextMenu(FMenuBuilder& MenuBuilder, const USNAssetBase* Asset, FSNSequencerContext* Context) override final;

    virtual void OnContextMenu(FMenuBuilder& MenuBuilder, const USNAssetBase* Asset, FSNSequencerContext* Context) {}

protected:
    FSlateColor BackgroundColor;
    FSlateColor BottomColor;
    FSlateIcon Icon;
    FString TypeName;
    FText TooltipText;
};
