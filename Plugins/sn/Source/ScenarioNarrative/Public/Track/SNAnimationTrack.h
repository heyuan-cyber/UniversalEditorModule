// AnimationTrack.h
// @author : xiangyp
// @date : 2025/05/07 17:31:15
// @desc : 动画轨道

#pragma once
#include "SNTrackBase.h"
#include "SNAnimationTrack.generated.h"

UCLASS()
class SCENARIONARRATIVE_API USNAnimationTrack : public USNTrackBase
{
    GENERATED_BODY()

public:
    virtual ~USNAnimationTrack() override = default;
};
