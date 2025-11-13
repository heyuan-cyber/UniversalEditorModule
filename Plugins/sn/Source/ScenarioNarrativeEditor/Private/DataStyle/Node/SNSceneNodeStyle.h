#pragma once
#include "DataStyle/SNNodeStyle.h"

class FSNSceneNodeStyle : public FSNNodeStyle
{
public:

    FSNSceneNodeStyle();

    virtual ~FSNSceneNodeStyle() override = default;

    virtual FString GetTypeName() const override;

    virtual FText GetTooltipText(const TObjectPtr<USNAssetBase>& Asset) const override;

    virtual void OnContextMenu(FMenuBuilder& MenuBuilder, const USNAssetBase* Asset, FSNSequencerContext* Context) override;

};

