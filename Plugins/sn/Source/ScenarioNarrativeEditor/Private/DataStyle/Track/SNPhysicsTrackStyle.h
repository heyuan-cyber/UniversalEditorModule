// PhysicsTrackStyle.h
// @author : xiangyp
// @date : 2025/05/12 16:21:15
// @desc :

#pragma once
#include "DataStyle/SNTrackStyle.h"

class FSNPhysicsTrackStyle : public FSNTrackStyle
{
public:
    FSNPhysicsTrackStyle();
    virtual ~FSNPhysicsTrackStyle() override = default;

    virtual FString GetTypeName() const override;
};
