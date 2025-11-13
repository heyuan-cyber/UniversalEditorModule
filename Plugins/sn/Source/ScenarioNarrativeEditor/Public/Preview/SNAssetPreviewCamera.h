// AssetPreviewPlayer.h
// @author : xiangyp
// @date : 2025/04/28 14:22:23
// @desc : 编辑器预览播放器

#pragma once
#include "SNAssetPreviewBase.h"
#include "Camera/CameraComponent.h"

class UNiagaraSystem;
class UNiagaraComponent;
class FSNAssetPreviewScene;
class USNNodeBase;
struct FSNPreviewContext;
class ASNPreviewCamera;
class USNSequencer;

class SCENARIONARRATIVEEDITOR_API FSNAssetPreviewCamera : public TSharedFromThis<FSNAssetPreviewCamera>, public  FSNAssetPreviewBase
{
public:
    FSNAssetPreviewCamera();
    FSNAssetPreviewCamera(TWeakPtr<FSNPreviewContext>  PreviewContext);
    virtual ~FSNAssetPreviewCamera() override;

    void InitPreviewResource(TWeakPtr<FSNPreviewContext> Context) override;

    ASNPreviewCamera* PreviewCamera;
    UCameraComponent* CameraComponent;

    ASNPreviewCamera* SpawnCamera(UWorld* InWorld, UClass* CameraType);
    ASNPreviewCamera* SpawnCamera(const FSNAssetPreviewScene* PreviewScene, UClass* CameraType);
    void DestroyCamera() const;
    ASNPreviewCamera* CreatePreviewCamera();

private:
    TSharedPtr<class FSNCameraPool> CameraPool;

    TWeakPtr<FSNPreviewContext>  PreviewContext;
};
