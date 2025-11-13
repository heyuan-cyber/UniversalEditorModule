// AnimationNodeStyle.h
// @author : xiangyp
// @date : 2025/05/07 16:44:07
// @desc : 动画节点样式

#pragma once
#include "DataStyle/SNNodeStyle.h"

class FSNCameraNodeStyle : public FSNNodeStyle
{
public:
    FSNCameraNodeStyle();
    virtual ~FSNCameraNodeStyle() override = default;

    virtual FString GetTypeName() const override;

    virtual FText GetTooltipText(const TObjectPtr<USNAssetBase>& Asset) const override;

    virtual void OnContextMenu(FMenuBuilder& MenuBuilder, const USNAssetBase* Asset, FSNSequencerContext* Context) override;
};
