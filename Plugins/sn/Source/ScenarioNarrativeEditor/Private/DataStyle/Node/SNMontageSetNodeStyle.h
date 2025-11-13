// MontageSetNodeStyle.h
// @author : xiangyp
// @date : 2025/05/13 13:39:09
// @desc :

#pragma once
#include "DataStyle/SNNodeStyle.h"

class FSNMontageSetNodeStyle : public FSNNodeStyle
{
public:
    FSNMontageSetNodeStyle();
    virtual ~FSNMontageSetNodeStyle() override = default;

    virtual FString GetTypeName() const override;

    virtual FText GetTooltipText(const TObjectPtr<USNAssetBase>& Asset) const override;
};
