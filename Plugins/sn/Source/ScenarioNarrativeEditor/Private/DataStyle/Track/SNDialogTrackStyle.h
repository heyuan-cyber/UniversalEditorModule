#pragma once

#include "DataStyle/SNTrackStyle.h"

class FSNDialogTrackStyle : public FSNTrackStyle
{
public:
    FSNDialogTrackStyle();
    virtual ~FSNDialogTrackStyle() override = default;

    virtual FString GetTypeName() const override;
};
