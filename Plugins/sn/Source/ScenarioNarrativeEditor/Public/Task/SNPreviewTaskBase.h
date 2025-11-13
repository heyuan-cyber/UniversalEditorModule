// PreviewTaskBase.h
// @author : xiangyp
// @date : 2025/04/28 15:32:41
// @desc :

#pragma once
#include "SNPreviewTaskRegistry.h"
#include "Preview/SNPreviewManager.h"

struct FSNPreviewContext;
class FSNPreviewTaskBase;

#define AUTO_REGISTER_TASK(NodeClass, TaskClass) \
static TTaskAutoRegister<TaskClass> TaskClass##_AutoReg(NodeClass::StaticClass());

template<typename T>
struct TTaskAutoRegister
{
    TTaskAutoRegister(UClass* NodeClass)
    {
        FSNPreviewTaskRegistry::Get().RegisteredTask.Add(NodeClass, std::move(new T()));
    }
};


class SCENARIONARRATIVEEDITOR_API FSNPreviewTaskBase
{
public:
    FSNPreviewTaskBase() = default;
    virtual ~FSNPreviewTaskBase() = default;

    // call when begin preview (not work on track)
    virtual void Enter(const FSNPreviewContext* Context) = 0;
    // call during preview
    virtual void Sample(const FSNPreviewManager* Context, float CurrentTime, float PrevTime) = 0;
    // call end preview (not work on track)
    virtual void Exit(const FSNPreviewContext* Context) = 0;

    // call when editor closed, all asset will execute even not sample
    virtual void Close(const FSNPreviewContext* Context) {}
};
