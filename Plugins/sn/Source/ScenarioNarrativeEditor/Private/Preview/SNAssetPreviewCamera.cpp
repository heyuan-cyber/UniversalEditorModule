#include "Preview/SNAssetPreviewCamera.h"
#include "Camera/CameraActor.h"
#include "Node/SNCameraNode.h"
#include "Preview/SNPreviewCamera.h"
#include "Preview/SNPreviewManager.h"
#include "PreviewScene/SNAssetPreviewScene.h"
#include "Preview/SNPreviewContext.h"


FSNAssetPreviewCamera::FSNAssetPreviewCamera()
{
}

FSNAssetPreviewCamera::FSNAssetPreviewCamera(TWeakPtr<FSNPreviewContext> PreviewContext)
{
    this->PreviewContext = PreviewContext;
    PreviewCamera = nullptr;
    CameraComponent = nullptr;
    RegistryToManager(USNCameraNode)
}

void FSNAssetPreviewCamera::InitPreviewResource(TWeakPtr<FSNPreviewContext> Context)
{
    this->PreviewContext = PreviewContext;
    PreviewCamera = nullptr;
    CameraComponent = nullptr;
    RegistryToManager(USNCameraNode)
}

FSNAssetPreviewCamera::~FSNAssetPreviewCamera()
{
    if(PreviewCamera)
        PreviewCamera->Destroy();
}

ASNPreviewCamera* FSNAssetPreviewCamera::SpawnCamera(UWorld* InWorld, UClass* CameraType)
{
    return CreatePreviewCamera();
}

ASNPreviewCamera* FSNAssetPreviewCamera::SpawnCamera(const FSNAssetPreviewScene* PreviewScene, UClass* CameraType)
{
    return SpawnCamera(PreviewScene->GetWorld(), CameraType);
}

void FSNAssetPreviewCamera::DestroyCamera() const
{
    PreviewCamera->Destroy();
}

// 创建预览相机
ASNPreviewCamera* FSNAssetPreviewCamera::CreatePreviewCamera()
{
    if (!PreviewContext.Pin()->World || PreviewCamera)
    {
        return PreviewCamera;
    }

    // 生成相机Actor
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    PreviewCamera = PreviewContext.Pin()->World->SpawnActor<ASNPreviewCamera>(FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

    if (PreviewCamera)
    {
        // 获取相机组件
        CameraComponent = PreviewCamera->GetCameraComponent();

        // 设置默认相机参数
        CameraComponent->SetFieldOfView(90.0f);
        CameraComponent->SetConstraintAspectRatio(false);
    }

    return PreviewCamera;
}

