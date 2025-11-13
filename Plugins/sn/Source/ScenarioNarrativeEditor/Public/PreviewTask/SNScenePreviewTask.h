#pragma once
#include "Task/SNPreviewTaskBase.h"

class FSNScenePreviewTask: public FSNPreviewTaskBase
{
public:

    FSNScenePreviewTask() = default;

    virtual ~FSNScenePreviewTask() override = default;

    virtual void Enter(const FSNPreviewContext* Context) override;

    virtual void Sample(const FSNPreviewManager* Context, float CurrentTime, float PrevTime) override;

    virtual void Exit(const FSNPreviewContext* Context) override;

};
