#pragma once
#include "DataStyle/SNTrackStyle.h"
#include "Track/SNPerformerTrack.h"

class FSNPerformerTrackStyle : public FSNTrackStyle
{
public:
    FSNPerformerTrackStyle();
    virtual ~FSNPerformerTrackStyle() override = default;

    virtual FString GetTypeName() const override;
};
AUTO_REGISTER_STYLE(USNPerformerTrack, FSNPerformerTrackStyle)
