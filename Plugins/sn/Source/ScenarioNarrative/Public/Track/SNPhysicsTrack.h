// PhysicsTrack.h
// @author : xiangyp
// @date : 2025/05/12 15:52:58
// @desc : 物理轨道

#pragma once
#include "SNTrackBase.h"
#include "SNPhysicsTrack.generated.h"

UCLASS(Blueprintable)
class SCENARIONARRATIVE_API USNPhysicsTrack : public USNTrackBase
{
    GENERATED_BODY()

public:
    USNPhysicsTrack() = default;
    virtual ~USNPhysicsTrack() override = default;
};
