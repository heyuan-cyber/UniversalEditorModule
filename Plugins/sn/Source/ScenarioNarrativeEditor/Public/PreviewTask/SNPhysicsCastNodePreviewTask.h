// PhysicsCastNodePreviewTask.h
// @author : xiangyp
// @date : 2025/05/16 17:23:22
// @desc :

#pragma once
#include "Task/SNPreviewTaskBase.h"

class USNPhysicsCastNode;

class FSNPhysicsCastNodePreviewTask : public FSNPreviewTaskBase
{
public:
    FSNPhysicsCastNodePreviewTask();
    virtual ~FSNPhysicsCastNodePreviewTask() override;

    virtual void Enter(const FSNPreviewContext* Context) override;
    virtual void Sample(const FSNPreviewManager* Context, float CurrentTime, float PrevTime) override;
    virtual void Exit(const FSNPreviewContext* Context) override;

private:
    TSharedPtr<FCollisionShape> Shape = nullptr;

    void UpdateShape(const USNPhysicsCastNode* Node) const;

    void DrawContinueColliderMesh(const USNPhysicsCastNode* Node, const FSNPreviewContext* Context);
};
