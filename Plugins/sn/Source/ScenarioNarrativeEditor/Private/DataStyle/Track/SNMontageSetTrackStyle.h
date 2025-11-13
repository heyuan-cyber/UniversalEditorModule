#pragma once
#include "DataStyle/SNTrackStyle.h"

class FSNMontageSetTrackStyle : public FSNTrackStyle
{
public:
    FSNMontageSetTrackStyle();
    virtual ~FSNMontageSetTrackStyle() override = default;

    virtual FString GetTypeName() const override;
};
