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
class ASNPreviewSpline;
class USNSequencer;
class USNSplineCameraNode;

class SCENARIONARRATIVEEDITOR_API FSNPreviewSplineObjectPool
{
public:
    FSNPreviewSplineObjectPool();
    ~FSNPreviewSplineObjectPool();

    // 从对象池获取一个ASNPreviewSpline实例
    ASNPreviewSpline* AcquireSpline(TObjectPtr<UWorld> World);

    // 将ASNPreviewSpline实例归还到对象池
    void ReleaseSpline(ASNPreviewSpline* Spline);

    // 清空对象池
    void ClearPool();

    // 预创建一定数量的对象
    void PreWarm(TObjectPtr<UWorld> World, int32 Count);

private:
    UWorld* WorldContext;
    TArray<ASNPreviewSpline*> AvailableSplines;
    TArray<ASNPreviewSpline*> ActiveSplines;

    // 创建新的ASNPreviewSpline实例
    ASNPreviewSpline* CreateNewSpline(TObjectPtr<UWorld> World);
};

class SCENARIONARRATIVEEDITOR_API FSNAssetPreviewSpline : public TSharedFromThis<FSNAssetPreviewSpline>, FSNAssetPreviewBase
{
public:
    FSNAssetPreviewSpline(TWeakPtr<FSNPreviewContext>  PreviewContext);
    virtual ~FSNAssetPreviewSpline() override;

    ASNPreviewSpline* AcquireSplineForNode(USNSplineCameraNode* SplineNode);
    void ReleaseSplineForNode(USNSplineCameraNode* SplineNode);
    void OnSplineNodeRemoved(USNSplineCameraNode* SplineNode);
    ASNPreviewSpline* GetSplineForNode(USNSplineCameraNode* SplineNode) const;

private:
    TWeakPtr<FSNPreviewContext>  PreviewContext;

    TSharedPtr<FSNPreviewSplineObjectPool> SplineObjectPool;
    TMap<USNSplineCameraNode*, ASNPreviewSpline*> NodeToSplineMap;
};
