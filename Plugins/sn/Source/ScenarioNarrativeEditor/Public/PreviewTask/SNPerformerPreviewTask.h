#pragma once
#include "Task/SNPreviewTaskBase.h"
#include "Node/SNPerformerNode.h"

class FSNPerformerPreviewTask: public FSNPreviewTaskBase
{
public:

    FSNPerformerPreviewTask() = default;

    virtual ~FSNPerformerPreviewTask() override = default;

    virtual void Enter(const FSNPreviewContext* Context) override;

    virtual void Sample(const FSNPreviewManager* Context, float CurrentTime, float PrevTime) override;

    virtual void Exit(const FSNPreviewContext* Context) override;

};
AUTO_REGISTER_TASK(USNPerformerNode, FSNPerformerPreviewTask);
