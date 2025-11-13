// AnimationNodePreviewTask.h
// @author : xiangyp
// @date : 2025/04/28 19:43:05
// @desc : 动画片段编辑器预览逻辑

#pragma once
#include "Task/SNPreviewTaskBase.h"

struct FSNPreviewContext;

class FSNAnimationNodePreviewTask : public FSNPreviewTaskBase
{
public:
    FSNAnimationNodePreviewTask() = default;
    virtual ~FSNAnimationNodePreviewTask() override = default;

    virtual void Enter(const FSNPreviewContext* Context) override;
    virtual void Sample(const FSNPreviewManager* Context, float CurrentTime, float PrevTime) override;
    virtual void Exit(const FSNPreviewContext* Context) override;

private:
    TMap<const FSNPreviewContext*, TArray<TObjectPtr<USceneComponent>>> AttachedComponents;

    bool EnsureSkeleton(const FSNPreviewContext* Context) const;
};
