// ActorPool.h
// @author : xiangyp
// @date : 2025/05/07 13:50:56
// @desc : 简易Actor对象池

#pragma once

class FSNActorPool final
{
public:
    FSNActorPool() = default;
    ~FSNActorPool() = default;

    AActor* GetOne(UWorld* World, UClass* ActorType);

    template <typename T = AActor>
    T* GetOne(UWorld* World);

    template <typename T = AActor>
    T* GetOne();

    bool BackOne(AActor* Actor);

    void Cleanup();

private:
    // 未分配的对象
    TMap<UClass*, TArray<TObjectPtr<AActor>>> UseableActors;
    // 已分配的对象
    TMap<UClass*, TArray<TObjectPtr<AActor>>> AllocatedActors;
};

template <typename T>
T* FSNActorPool::GetOne(UWorld* World)
{
    return Cast<T>(GetOne(World, T::StaticClass()));
}

template <typename T>
T* FSNActorPool::GetOne()
{
    return Cast<T>(GetOne(nullptr, T::StaticClass()));
}
