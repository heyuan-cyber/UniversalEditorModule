// VfxTrackStyle.h
// @author : xiangyp
// @date : 2025/05/20 15:23:44
// @desc :

#pragma once
#include "DataStyle/SNTrackStyle.h"

class FSNVfxTrackStyle : public FSNTrackStyle
{
public:
    FSNVfxTrackStyle();
    virtual ~FSNVfxTrackStyle() override = default;

    virtual FString GetTypeName() const override;
};
