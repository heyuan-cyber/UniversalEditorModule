#include "Preview/SNPreviewActor.h"
#include "Animation/DebugSkelMeshComponent.h"

ASNPreviewOnlyActor::ASNPreviewOnlyActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASNPreviewOnlyActor::Reset()
{
    Super::Reset();

    TSet<UActorComponent*> Components = GetComponents();
    // 把组件都干掉
    for (auto* Comp : Components)
    {
        RemoveOwnedComponent(Comp);
        Comp->DestroyComponent();
    }
}

ASNPreviewActor::ASNPreviewActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

ASNPreviewActor::~ASNPreviewActor()
{
    OnPostEditMove = nullptr;
}


void ASNPreviewActor::PostEditMove(bool bFinished)
{
    if (bFinished) // 松开鼠标时才记录
    {
        if (OnPostEditMove)
        {
            OnPostEditMove(this);
        }
    }

    //Super::PostEditMove(bFinished);
}
