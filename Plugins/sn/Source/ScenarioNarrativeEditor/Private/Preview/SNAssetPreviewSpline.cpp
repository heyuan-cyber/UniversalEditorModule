#include "Preview/SNAssetPreviewSpline.h"
#include "Camera/CameraActor.h"
#include "Node/SNCameraNode.h"
#include "Node/SNSplineCameraNode.h"
#include "Preview/SNPreviewSpline.h"
#include "Preview/SNPreviewManager.h"
#include "Preview/SNPreviewSpline.h"
#include "PreviewScene/SNAssetPreviewScene.h"
#include "Preview/SNPreviewContext.h"

FSNPreviewSplineObjectPool::FSNPreviewSplineObjectPool()
{
}

FSNPreviewSplineObjectPool::~FSNPreviewSplineObjectPool()
{
    ClearPool();
}

ASNPreviewSpline* FSNPreviewSplineObjectPool::AcquireSpline(TObjectPtr<UWorld> World)
{
    ASNPreviewSpline* Spline = nullptr;

    // 先从可用池中获取
    if (AvailableSplines.Num() > 0)
    {
        Spline = AvailableSplines.Pop();
    }
    else
    {
        // 如果没有可用对象，创建新对象
        Spline = CreateNewSpline(World);
    }

    if (Spline)
    {
        // 激活对象
        Spline->SetActorHiddenInGame(false);
        Spline->SetActorEnableCollision(true);
        Spline->SetActorTickEnabled(true);

        // 添加到活跃列表
        ActiveSplines.Add(Spline);
    }

    return Spline;
}

void FSNPreviewSplineObjectPool::ReleaseSpline(ASNPreviewSpline* Spline)
{
    if (!IsValid(Spline) || !ActiveSplines.Contains(Spline))
    {
        return;
    }

    // 禁用对象
    Spline->SetActorHiddenInGame(true);
    Spline->SetActorEnableCollision(false);
    Spline->SetActorTickEnabled(false);

    // 从活跃列表移除
    ActiveSplines.Remove(Spline);

    // 添加到可用池
    AvailableSplines.Add(Spline);
}

void FSNPreviewSplineObjectPool::ClearPool()
{
    // 销毁所有对象
    for (ASNPreviewSpline* Spline : AvailableSplines)
    {
        if (IsValid(Spline))
        {
            Spline->Destroy();
        }
    }

    for (ASNPreviewSpline* Spline : ActiveSplines)
    {
        if (IsValid(Spline))
        {
            Spline->Destroy();
        }
    }

    AvailableSplines.Empty();
    ActiveSplines.Empty();
}

void FSNPreviewSplineObjectPool::PreWarm(TObjectPtr<UWorld> World, int32 Count)
{
    for (int32 i = 0; i < Count; ++i)
    {
        ASNPreviewSpline* NewSpline = CreateNewSpline(World);
        if (NewSpline)
        {
            // 初始状态设为禁用
            NewSpline->SetActorHiddenInGame(true);
            NewSpline->SetActorEnableCollision(false);
            NewSpline->SetActorTickEnabled(false);

            AvailableSplines.Add(NewSpline);
        }
    }
}

ASNPreviewSpline* FSNPreviewSplineObjectPool::CreateNewSpline(TObjectPtr<UWorld> World)
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParams.ObjectFlags = RF_Transactional; //RF_Transient; // 设置为临时对象，不被保存

    return World->SpawnActor<ASNPreviewSpline>(
        ASNPreviewSpline::StaticClass(),
        FVector::ZeroVector,
        FRotator::ZeroRotator,
        SpawnParams
    );
}

// 获取节点对应的样条线（如果存在）
ASNPreviewSpline* FSNAssetPreviewSpline::GetSplineForNode(USNSplineCameraNode* SplineNode) const
{
    if (!SplineNode)
    {
        return nullptr;
    }

    ASNPreviewSpline* const* SplinePtr = NodeToSplineMap.Find(SplineNode);
    return SplinePtr ? *SplinePtr : nullptr;
}


FSNAssetPreviewSpline::FSNAssetPreviewSpline(TWeakPtr<FSNPreviewContext> PreviewContext)
{
    this->PreviewContext = PreviewContext;


    SplineObjectPool = MakeShared<FSNPreviewSplineObjectPool>();
    // // 预创建一定数量的对象
    // SplineObjectPool->PreWarm(PreviewContext.Pin()->World, 5);

    RegistryToManager(USNSplineCameraNode)
}

FSNAssetPreviewSpline::~FSNAssetPreviewSpline()
{

}

// 添加管理SplineNode和PreviewSpline对应关系的方法
ASNPreviewSpline* FSNAssetPreviewSpline::AcquireSplineForNode(USNSplineCameraNode* SplineNode)
{
    if (!SplineObjectPool.IsValid() || !SplineNode)
    {
        return nullptr;
    }

    ASNPreviewSpline* Spline = SplineObjectPool->AcquireSpline(PreviewContext.Pin()->World);
    if (Spline)
    {
        NodeToSplineMap.Add(SplineNode, Spline);
    }

    return Spline;
}

void FSNAssetPreviewSpline::ReleaseSplineForNode(USNSplineCameraNode* SplineNode)
{
    if (!SplineObjectPool.IsValid() || !SplineNode)
    {
        return;
    }

    ASNPreviewSpline** SplinePtr = NodeToSplineMap.Find(SplineNode);
    if (SplinePtr && *SplinePtr)
    {
        SplineObjectPool->ReleaseSpline(*SplinePtr);
        NodeToSplineMap.Remove(SplineNode);
    }
}

// 当节点被删除时调用此方法
void FSNAssetPreviewSpline::OnSplineNodeRemoved(USNSplineCameraNode* SplineNode)
{
    ReleaseSplineForNode(SplineNode);
}

