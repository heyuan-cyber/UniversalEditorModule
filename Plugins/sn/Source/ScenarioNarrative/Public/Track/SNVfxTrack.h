// VfxTrack.h
// @author : xiangyp
// @date : 2025/05/20 13:40:32
// @desc : 特效轨道

#pragma once
#include "SNTrackBase.h"
#include "SNVfxTrack.generated.h"

UCLASS(Blueprintable)
class SCENARIONARRATIVE_API USNVfxTrack : public USNTrackBase
{
    GENERATED_BODY()

public:
    USNVfxTrack() = default;
    virtual ~USNVfxTrack() override = default;

};
