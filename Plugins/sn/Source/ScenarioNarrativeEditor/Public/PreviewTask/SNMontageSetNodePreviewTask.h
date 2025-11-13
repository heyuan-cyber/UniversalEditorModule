// MontageSetNodePreviewTask.h
// @author : xiangyp
// @date : 2025/05/13 13:52:18
// @desc :

#pragma once
#include "Task/SNPreviewTaskBase.h"

class FSNMontageSetNodePreviewTask : public FSNPreviewTaskBase
{
public:
    virtual void Enter(const FSNPreviewContext* Context) override;
    virtual void Sample(const FSNPreviewManager* Context, float CurrentTime, float PrevTime) override;
    virtual void Exit(const FSNPreviewContext* Context) override;

private:
    TMap<const FSNPreviewContext*, TArray<TObjectPtr<USceneComponent>>> AttachedComponents;

    bool EnsureSkeleton(const FSNPreviewContext* Context) const;
};
