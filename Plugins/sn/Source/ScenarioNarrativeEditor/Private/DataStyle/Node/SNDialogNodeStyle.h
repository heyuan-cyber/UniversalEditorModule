#pragma once
#include "DataStyle/SNNodeStyle.h"

class FSNDialogNodeStyle : public FSNNodeStyle
{
public:
    FSNDialogNodeStyle();

    virtual ~FSNDialogNodeStyle() override = default;

    virtual FString GetTypeName() const override;

    virtual FText GetTooltipText(const TObjectPtr<USNAssetBase>& Asset) const override;

    virtual void OnContextMenu(FMenuBuilder& MenuBuilder, const USNAssetBase* Asset, FSNSequencerContext* Context) override;
};
