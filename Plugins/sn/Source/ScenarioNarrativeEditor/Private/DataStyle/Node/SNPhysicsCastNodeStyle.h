// PhysicsCastNodeStyle.h
// @author : xiangyp
// @date : 2025/05/16 17:08:47
// @desc : 投射节点样式

#pragma once
#include "DataStyle/SNNodeStyle.h"
#include "DataStyle/SNStyleBase.h"

class FSNPhysicsCastNodeStyle : public FSNNodeStyle
{
public:
    FSNPhysicsCastNodeStyle();

    virtual FString GetTypeName() const override;
    virtual FText GetTooltipText(const TObjectPtr<USNAssetBase>& Asset) const override;

    virtual void OnContextMenu(FMenuBuilder& MenuBuilder, const USNAssetBase* Asset, FSNSequencerContext* Context) override;
};
