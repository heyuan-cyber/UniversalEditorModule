// InputTrack.h
// @author : xiangyp
// @date : 2025/06/05 16:41:21
// @desc : 输入控制轨道

#pragma once
#include "SNTrackBase.h"
#include "SNInputTrack.generated.h"

UCLASS(MinimalAPI)
class USNInputTrack : public USNTrackBase
{
    GENERATED_BODY()
public:
    USNInputTrack() = default;
    virtual ~USNInputTrack() override = default;
};
