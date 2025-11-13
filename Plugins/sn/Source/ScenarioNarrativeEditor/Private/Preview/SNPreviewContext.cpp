#include "Preview/SNPreviewContext.h"

// 原始指针的获取实现
template<typename T>
T* FSNPreviewContext::GetResource() const
{
    if constexpr (std::is_same_v<T, FSNAssetPreviewScene>)
    {
        return PreviewScene;
    }
    else if constexpr (std::is_same_v<T, FSNAssetPreviewSpline>)
    {
        return PreviewSpline;
    }
    else if constexpr (std::is_same_v<T, SSNAssetViewport>)
    {
        return Viewport.Pin().Get();
    }
    else if constexpr (std::is_same_v<T, FSNAssetPreviewCamera>)
    {
        return Camera.Get();
    }
    else if constexpr (std::is_same_v<T, FSNAssetPreviewHUD>)
    {
        return HUD.Get();
    }
    else if constexpr (std::is_same_v<T, FSNAssetPreviewPerformer>)
    {
        return Performer.Get();
    }
    else if constexpr (std::is_same_v<T, USNSequencer>)
    {
        return AssetSequence.Get();
    }
    else if constexpr (std::is_same_v<T, AActor>)
    {
        return Actor.Get();
    }
    else if constexpr (std::is_same_v<T, UWorld>)
    {
        return World.Get();
    }
    else if constexpr (std::is_same_v<T, USNNodeBase>)
    {
        return Node.Get();
    }
    else if constexpr (std::is_same_v<T, USNAssetBase>)
    {
        return Track.Get();
    }
    else
    {
        return nullptr;
    }
}

// 共享指针的获取实现
template<typename T>
TSharedPtr<T> FSNPreviewContext::GetSharedResource() const
{
    if constexpr (std::is_same_v<T, FSNAssetPreviewCamera>)
    {
        return Camera;
    }
    else if constexpr (std::is_same_v<T, FSNAssetPreviewHUD>)
    {
        return HUD;
    }
    else if constexpr (std::is_same_v<T, FSNAssetPreviewPerformer>)
    {
        return Performer;
    }
    else
    {
        return TSharedPtr<T>();
    }
}

// 弱指针的获取实现
template<typename T>
TWeakPtr<T> FSNPreviewContext::GetWeakResource() const
{
    if constexpr (std::is_same_v<T, SSNAssetViewport>)
    {
        return Viewport;
    }
    return TWeakPtr<T>();
}

// 对象指针的获取实现
template<typename T>
TObjectPtr<T> FSNPreviewContext::GetObjectResource() const
{
    if constexpr (std::is_same_v<T, USNSequencer>)
    {
        return AssetSequence;
    }
    else if constexpr (std::is_same_v<T, AActor>)
    {
        return Actor;
    }
    else if constexpr (std::is_same_v<T, UWorld>)
    {
        return World;
    }
    else if constexpr (std::is_same_v<T, USNNodeBase>)
    {
        return Node;
    }
    else if constexpr (std::is_same_v<T, USNAssetBase>)
    {
        return Track;
    }
    else
    {
        return TObjectPtr<T>();
    }
}

// 原始指针的设置实现
template<typename T>
void FSNPreviewContext::SetResource(T* NewResource)
{
    if constexpr (std::is_same_v<T, FSNAssetPreviewScene>)
    {
        PreviewScene = NewResource;
    }
    else if constexpr (std::is_same_v<T, FSNAssetPreviewSpline>)
    {
        PreviewSpline = NewResource;
    }
    else if constexpr (std::is_same_v<T, FSNAssetPreviewCamera>)
    {
        Camera = TSharedPtr<FSNAssetPreviewCamera>(NewResource);
    }
    else if constexpr (std::is_same_v<T, FSNAssetPreviewHUD>)
    {
        HUD = TSharedPtr<FSNAssetPreviewHUD>(NewResource);
    }
    else if constexpr (std::is_same_v<T, FSNAssetPreviewPerformer>)
    {
        Performer = TSharedPtr<FSNAssetPreviewPerformer>(NewResource);
    }
    else if constexpr (std::is_same_v<T, USNSequencer>)
    {
        AssetSequence = NewResource;
    }
    else if constexpr (std::is_same_v<T, AActor>)
    {
        Actor = NewResource;
    }
    else if constexpr (std::is_same_v<T, UWorld>)
    {
        World = NewResource;
    }
    else if constexpr (std::is_same_v<T, USNNodeBase>)
    {
        Node = NewResource;
    }
    else if constexpr (std::is_same_v<T, USNAssetBase>)
    {
        Track = NewResource;
    }
}

// 共享指针的设置实现
template<typename T>
void FSNPreviewContext::SetResource(const TSharedPtr<T>& NewResource)
{
    if constexpr (std::is_same_v<T, FSNAssetPreviewCamera>)
    {
        Camera = NewResource;
    }
    else if constexpr (std::is_same_v<T, FSNAssetPreviewHUD>)
    {
        HUD = NewResource;
    }
    else if constexpr (std::is_same_v<T, FSNAssetPreviewPerformer>)
    {
        Performer = NewResource;
    }
    else if constexpr (std::is_same_v<T, SSNAssetViewport>)
    {
        Viewport = NewResource;
    }
}

// 弱指针的设置实现
template<typename T>
void FSNPreviewContext::SetResource(const TWeakPtr<T>& NewResource)
{
    if constexpr (std::is_same_v<T, SSNAssetViewport>)
    {
        Viewport = NewResource;
    }
}

// 对象指针的设置实现
template<typename T>
void FSNPreviewContext::SetResource(const TObjectPtr<T>& NewResource)
{
    if constexpr (std::is_same_v<T, USNSequencer>)
    {
        AssetSequence = NewResource;
    }
    else if constexpr (std::is_same_v<T, AActor>)
    {
        Actor = NewResource;
    }
    else if constexpr (std::is_same_v<T, UWorld>)
    {
        World = NewResource;
    }
    else if constexpr (std::is_same_v<T, USNNodeBase>)
    {
        Node = NewResource;
    }
    else if constexpr (std::is_same_v<T, USNAssetBase>)
    {
        Track = NewResource;
    }
}

// 显式实例化模板
// 原始指针获取
template FSNAssetPreviewCamera* FSNPreviewContext::GetResource() const;
template FSNAssetPreviewHUD* FSNPreviewContext::GetResource() const;
template FSNAssetPreviewScene* FSNPreviewContext::GetResource() const;
template FSNAssetPreviewSpline* FSNPreviewContext::GetResource() const;
template SSNAssetViewport* FSNPreviewContext::GetResource() const;
template USNSequencer* FSNPreviewContext::GetResource() const;
template AActor* FSNPreviewContext::GetResource() const;
template UWorld* FSNPreviewContext::GetResource() const;
template USNNodeBase* FSNPreviewContext::GetResource() const;
template USNAssetBase* FSNPreviewContext::GetResource() const;

// 共享指针获取
template TSharedPtr<FSNAssetPreviewCamera> FSNPreviewContext::GetSharedResource() const;
template TSharedPtr<FSNAssetPreviewHUD> FSNPreviewContext::GetSharedResource() const;
template TSharedPtr<FSNAssetPreviewPerformer> FSNPreviewContext::GetSharedResource() const;

// 弱指针获取
template TWeakPtr<SSNAssetViewport> FSNPreviewContext::GetWeakResource() const;

// 对象指针获取
template TObjectPtr<USNSequencer> FSNPreviewContext::GetObjectResource() const;
template TObjectPtr<AActor> FSNPreviewContext::GetObjectResource() const;
template TObjectPtr<UWorld> FSNPreviewContext::GetObjectResource() const;
template TObjectPtr<USNNodeBase> FSNPreviewContext::GetObjectResource() const;
template TObjectPtr<USNAssetBase> FSNPreviewContext::GetObjectResource() const;

// 原始指针设置
template void FSNPreviewContext::SetResource(FSNAssetPreviewCamera*);
template void FSNPreviewContext::SetResource(FSNAssetPreviewHUD*);
template void FSNPreviewContext::SetResource(FSNAssetPreviewScene*);
template void FSNPreviewContext::SetResource(FSNAssetPreviewSpline*);
template void FSNPreviewContext::SetResource(SSNAssetViewport*);
template void FSNPreviewContext::SetResource(USNSequencer*);
template void FSNPreviewContext::SetResource(AActor*);
template void FSNPreviewContext::SetResource(UWorld*);
template void FSNPreviewContext::SetResource(USNNodeBase*);
template void FSNPreviewContext::SetResource(USNAssetBase*);

// 共享指针设置
template void FSNPreviewContext::SetResource(const TSharedPtr<FSNAssetPreviewCamera>&);
template void FSNPreviewContext::SetResource(const TSharedPtr<FSNAssetPreviewHUD>&);
template void FSNPreviewContext::SetResource(const TSharedPtr<FSNAssetPreviewPerformer>&);
template void FSNPreviewContext::SetResource(const TSharedPtr<SSNAssetViewport>&);

// 弱指针设置
template void FSNPreviewContext::SetResource(const TWeakPtr<SSNAssetViewport>&);

// 对象指针设置
template void FSNPreviewContext::SetResource(const TObjectPtr<USNSequencer>&);
template void FSNPreviewContext::SetResource(const TObjectPtr<AActor>&);
template void FSNPreviewContext::SetResource(const TObjectPtr<UWorld>&);
template void FSNPreviewContext::SetResource(const TObjectPtr<USNNodeBase>&);
template void FSNPreviewContext::SetResource(const TObjectPtr<USNAssetBase>&);
