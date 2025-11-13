#include "SNActorPool.h"
#include "CoreMinimal.h"

bool FSNActorPool::BackOne(AActor* Actor)
{
    ensureMsgf(Actor, TEXT("actor pool cant handle empty actor"));

    if (!AllocatedActors.Contains(Actor->GetClass()))
    {
        return false;
    }

    if (!AllocatedActors[Actor->GetClass()].Contains(Actor))
    {
        return false;
    }

    Actor->Reset();
    Actor->SetIsTemporarilyHiddenInEditor(true);

    AllocatedActors[Actor->GetClass()].Remove(Actor);
    UseableActors[Actor->GetClass()].Add(Actor);
    return true;
}

AActor* FSNActorPool::GetOne(UWorld* World, UClass* ActorType)
{
    if (!UseableActors.Contains(ActorType))
    {
        UseableActors.Add(ActorType, {});
    }

    if (!AllocatedActors.Contains(ActorType))
    {
        AllocatedActors.Add(ActorType, {});
    }

    if (!World)
    {
        World = GEditor->GetWorld();
    }

    TObjectPtr<AActor> Actor = nullptr;
    if (!UseableActors[ActorType].IsEmpty())
    {
        const auto Pred = [World](const TObjectPtr<AActor>& Item) { return Item->GetWorld() == World; };
        if (auto* FindRet = UseableActors[ActorType].FindByPredicate(Pred))
        {
            Actor = *FindRet;
        }
    }

    if (!Actor)
    {
        Actor = World->SpawnActor(ActorType);
    }

    Actor->SetIsTemporarilyHiddenInEditor(false);

    AllocatedActors[ActorType].Add(Actor);
    return Actor;
}

void FSNActorPool::Cleanup()
{
    for (const auto& Pair : UseableActors)
    {
        for (const auto& Actor : Pair.Value)
        {
            Actor->Destroy();
        }
        UseableActors[Pair.Key].Empty();
    }

    for (const auto& Pair : AllocatedActors)
    {
        for (const auto& Actor : Pair.Value)
        {
            Actor->Destroy();
        }
        AllocatedActors[Pair.Key].Empty();
    }
}
