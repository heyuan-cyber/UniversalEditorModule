#pragma once
#define RegistryToManager(NodeClass) FSNPreviewManager::NodeToManager.Add(NodeClass::StaticClass(), this);
#include "SNPreviewManager.h"

struct FSNPreviewContext;

class FSNAssetPreviewBase
{

public:
    FSNAssetPreviewBase() = default;
    virtual ~FSNAssetPreviewBase() = default;

    virtual void InitPreviewResource(TWeakPtr<FSNPreviewContext> Context);

    // call when begin preview (not work on track)
    virtual void Enter(const FSNPreviewContext* Context);

    // call during preview
    virtual void Sample(const FSNPreviewManager* Context, float CurrentTime, float PrevTime);

    // call end preview (not work on track)
    virtual void Exit(const FSNPreviewContext* Context);

    // call when editor closed, all asset will execute even not sample
    virtual void Close(const FSNPreviewContext* Context) {}
};
