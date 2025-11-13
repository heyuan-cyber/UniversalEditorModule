#pragma once
#include "DataStyle/SNNodeStyle.h"
#include "Node/SNPerformerNode.h"

class FSNPerformerNodeStyle : public FSNNodeStyle
{
public:
    FSNPerformerNodeStyle();

    virtual ~FSNPerformerNodeStyle() override = default;

    virtual FString GetTypeName() const override;

    virtual FText GetTooltipText(const TObjectPtr<USNAssetBase>& Asset) const override;

    virtual void OnContextMenu(FMenuBuilder& MenuBuilder, const USNAssetBase* Asset, FSNSequencerContext* Context) override;
};
