#include "Preview/SNAssetPreviewBase.h"
#include "Preview/SNPreviewContext.h"
#include "Task/SNPreviewTaskBase.h"
#include "Task/SNPreviewTaskRegistry.h"

void FSNAssetPreviewBase::InitPreviewResource(TWeakPtr<FSNPreviewContext> Context)
{
}

void FSNAssetPreviewBase::Enter(const FSNPreviewContext* Context)
{
    if (auto Task = FSNPreviewTaskRegistry::Get().GetPreviewTask(Context->Node))
    {
        Task->Enter(Context);
    }
}

void FSNAssetPreviewBase::Sample(const FSNPreviewManager* Context, float CurrentTime, float PrevTime)
{
    if(CurrentTime == PrevTime) return;
    if (auto Task = FSNPreviewTaskRegistry::Get().GetPreviewTask(Context->PreviewContext->Node))
    {
        Task->Sample(Context, CurrentTime, PrevTime);
    }
}

void FSNAssetPreviewBase::Exit(const FSNPreviewContext* Context)
{
    if (auto Task = FSNPreviewTaskRegistry::Get().GetPreviewTask(Context->Node))
    {
        Task->Exit(Context);
    }
}
