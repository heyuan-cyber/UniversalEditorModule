#pragma once
#include "SEditorViewport.h"
#include "SNSequencerContext.h"
#include "Preview/SNPreviewContext.h"
#include "Preview/SNPreviewManager.h"

class FCameraPreviewClient;
class SCameraPreview;
class FSNAssetViewportClient;
class FSNSequencerContext;
class FSNAssetPreviewScene;

class SSNAssetViewport : public SEditorViewport
{
public:
    SLATE_BEGIN_ARGS(SSNAssetViewport) {}
        SLATE_ARGUMENT(TSharedPtr<FSNAssetPreviewScene>, PreviewScene)
        SLATE_ARGUMENT(TSharedPtr<FSNSequencerContext>, Context)
        SLATE_ARGUMENT(TSharedPtr<IDetailsView>, InspectorWidget)
        SLATE_ARGUMENT(TSharedPtr<FEditorModeTools>, InModeTools)

    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);
    virtual ~SSNAssetViewport() override;

    virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;

    TSharedPtr<FSNAssetPreviewScene> GetPreviewScene() const { return PreviewScene.Pin(); }
    FSNAssetPreviewCamera* GetPreviewCamera() const { return Context->PreviewManager->PreviewContext->GetResource<FSNAssetPreviewCamera>(); }

    virtual void BindCommands() override {}
    virtual void OnFocusViewportToSelection() override {}
    virtual void Tick( const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime ) override;

    void ShowObjectInspectorWidget(UObject* object) const;
    UObject* GetCurrentSelectedUObject();

    FORCEINLINE TSharedPtr<FEditorViewportClient> GetViewportClient() { return Client; }
    FSNAssetViewportClient* GetSNViewportClient();
    void AddViewportWidget(TSharedPtr<SWidget> Widget);

    void PreviewCamera(const ACameraActor* InCamera);
    void RemoveCameraPreview();
    void AddCameraPreview(const ACameraActor* InCamera);

protected:
    // 键盘事件 被成员变量SViewport ViewportWidget 的 OnKeyDown 先一步触发， 其又先路由到ViewportClient， 因此这个函数处于最低优先级触发
    // 若想接收到输入后优先响应UICommand， 需要自己在最先响应的Input Key事件中 路由到UICommand， 然后在路由到其他Widget上
    virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;


private:
    TWeakPtr<FSNAssetPreviewScene> PreviewScene;
    TSharedPtr<FSNSequencerContext> Context;
    TSharedPtr<IDetailsView> InspectorWidget;
    TSharedPtr<FEditorModeTools> InModeTools;
    TSharedPtr<FCameraPreviewClient> CameraPreviewClient;
    TSharedPtr<SCameraPreview> CameraPreviewWidget;
    TSharedPtr<FSceneViewport> ActorPreviewSceneViewport;
    ACameraActor* PreviewingCamera = nullptr;
};
