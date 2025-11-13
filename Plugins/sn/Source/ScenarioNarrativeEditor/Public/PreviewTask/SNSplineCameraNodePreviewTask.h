// AnimationNodePreviewTask.h
// @author : xiangyp
// @date : 2025/04/28 19:43:05
// @desc : 动画片段编辑器预览逻辑

#pragma once
#include "Task/SNPreviewTaskBase.h"

struct FSNPreviewContext;

class FSNSplineCameraNodePreviewTask : public FSNPreviewTaskBase
{
public:
    FSNSplineCameraNodePreviewTask() = default;
    virtual ~FSNSplineCameraNodePreviewTask() override = default;

    virtual void Enter(const FSNPreviewContext* Context) override;
    virtual void Sample(const FSNPreviewManager* Context, float CurrentTime, float PrevTime) override;
    virtual void Exit(const FSNPreviewContext* Context) override;

private:
    TMap<const FSNPreviewContext*, TArray<TObjectPtr<USceneComponent>>> AttachedComponents;

    int32 CachedPrevKeyFrameIndex = -1;
    int32 CachedNextKeyFrameIndex = -1;

    // 标记是否已经初始化
    bool bIsInitialized;
};
