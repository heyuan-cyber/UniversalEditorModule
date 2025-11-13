// BehaviorTrack.h
// @author : xiangyp
// @date : 2025/06/05 17:54:27
// @desc : 行为轨道

#pragma once
#include "SNTrackBase.h"
#include "SNBehaviorTrack.generated.h"

UCLASS(MinimalAPI)
class USNBehaviorTrack : public USNTrackBase
{
    GENERATED_BODY()
public:
    USNBehaviorTrack() = default;
    virtual ~USNBehaviorTrack() override = default;

};
