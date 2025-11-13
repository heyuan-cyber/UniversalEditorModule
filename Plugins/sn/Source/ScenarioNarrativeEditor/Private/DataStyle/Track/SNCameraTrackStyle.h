// AnimationTrackStyle.h
// @author : xiangyp
// @date : 2025/05/07 17:39:14
// @desc : 动画轨道样式

#pragma once
#include "DataStyle/SNTrackStyle.h"

class FSNCameraTrackStyle : public FSNTrackStyle
{
public:
    FSNCameraTrackStyle();
    virtual ~FSNCameraTrackStyle() override = default;

    virtual FString GetTypeName() const override;
};
