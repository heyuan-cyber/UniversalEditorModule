// PreviewActor.h
// @author : xiangyp
// @date : 2025/04/28 11:09:29
// @desc : 在预览场景中的预览角色

#pragma once
#include "Camera/CameraActor.h"
#include "SNPreviewCamera.generated.h"

UCLASS()
class SCENARIONARRATIVEEDITOR_API ASNPreviewCamera : public ACameraActor
{
    GENERATED_BODY()

public:
    ASNPreviewCamera();

protected:
};
