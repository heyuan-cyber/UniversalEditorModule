#pragma once
#include "Task/SNPreviewTaskBase.h"

class FSNHUDPreviewTask: public FSNPreviewTaskBase
{
public:

    FSNHUDPreviewTask() = default;

    virtual ~FSNHUDPreviewTask() override = default;

    virtual void Enter(const FSNPreviewContext* Context) override;

    virtual void Sample(const FSNPreviewManager* Context, float CurrentTime, float PrevTime) override;

    virtual void Exit(const FSNPreviewContext* Context) override;

};
