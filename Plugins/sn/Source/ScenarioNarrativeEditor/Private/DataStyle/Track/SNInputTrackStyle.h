// InputTrackStyle.h
// @author : xiangyp
// @date : 2025/06/05 19:48:38
// @desc : 输入轨道样式

#pragma once
#include "DataStyle/SNTrackStyle.h"

class FSNInputTrackStyle : public FSNTrackStyle
{
public:
    FSNInputTrackStyle();
    virtual ~FSNInputTrackStyle() override = default;

    virtual FString GetTypeName() const override;
    virtual float GetDesiredHeight() const override;
};
