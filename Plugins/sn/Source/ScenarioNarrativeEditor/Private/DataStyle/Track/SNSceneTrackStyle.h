#pragma once
#include "DataStyle/SNTrackStyle.h"

class FSNSceneTrackStyle : public FSNTrackStyle
{
public:
    FSNSceneTrackStyle();
    virtual ~FSNSceneTrackStyle() override = default;

    virtual FString GetTypeName() const override;
};
