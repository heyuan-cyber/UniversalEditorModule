// EditorDebugTrack.h
// @author : xiangyp
// @date : 2025/06/23 15:24:08
// @desc : 仅在编辑器下运行的轨道

#pragma once
#include "Track/SNTrackBase.h"
#include "SNEditorDebugTrack.generated.h"

UCLASS()
class SCENARIONARRATIVE_API USNEditorDebugTrack : public USNTrackBase
{
    GENERATED_BODY()
public:
    virtual ~USNEditorDebugTrack() override = default;

};
