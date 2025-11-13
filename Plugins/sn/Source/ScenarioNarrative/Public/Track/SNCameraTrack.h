// AnimationTrack.h
// @author : xiangyp
// @date : 2025/05/07 17:31:15
// @desc : 动画轨道

#pragma once
#include "SNTrackBase.h"
#include "SNCameraTrack.generated.h"

UCLASS()
class SCENARIONARRATIVE_API USNCameraTrack : public USNTrackBase
{
    GENERATED_BODY()

public:
    virtual ~USNCameraTrack() override = default;
    virtual TSharedPtr<FJsonObject> TrackDataToJson() const override;
};
