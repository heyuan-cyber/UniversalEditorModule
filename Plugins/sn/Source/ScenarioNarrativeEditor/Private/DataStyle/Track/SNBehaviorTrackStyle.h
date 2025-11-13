// BehaviorTrackStyle.h
// @author : xiangyp
// @date : 2025/06/05 20:16:23
// @desc : 行为轨道样式

#pragma once
#include "DataStyle/SNTrackStyle.h"

class FSNBehaviorTrackStyle : public FSNTrackStyle
{
public:
    FSNBehaviorTrackStyle();
    virtual ~FSNBehaviorTrackStyle() override = default;

    virtual FString GetTypeName() const override;
};
