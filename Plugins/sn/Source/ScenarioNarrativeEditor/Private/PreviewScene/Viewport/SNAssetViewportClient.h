#pragma once

class USNEditorEngine;
class FScenarioNarrativeEditorModule;
class FSNSequencerContext;
class SSNAssetViewport;
class FSNAssetPreviewScene;

class FSNAssetViewportClient : public FEditorViewportClient, public TSharedFromThis<FSNAssetViewportClient>
{
public:
    TObjectPtr<USNEditorEngine> SNEE;
private:
    // TODO:
    // 考虑集成CommonUI处理输入层级，暂时先使用标志位简单实现
    bool bIsFocusHUD = false;

    TWeakPtr<SSNAssetViewport> SNViewport;

    // 物体选择和变换相关成员变量
    AActor* SelectedActor = nullptr;
    USceneComponent* SelectedComponent = nullptr;

    UE::Widget::EWidgetMode WidgetMode = UE::Widget::WM_Translate;
    bool bTransactionActive = false;
    float LastExecTransactionTime = 0.0f;

    enum class ETransformType : uint8
    {
        World,
        Relative
    };

    ETransformType TransformType = ETransformType::World;

    bool bUseCustomCamera = false;
    float _DeltaSeconds = 0.0f;

public:
    FSNAssetViewportClient(TSharedPtr<FEditorModeTools> InModeTool,
                            const TSharedRef<SSNAssetViewport>& InThumbnailViewport,
                            const TSharedRef<FSNAssetPreviewScene>& InPreviewScene);
    virtual ~FSNAssetViewportClient() override;

    virtual void Draw(const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
    FVector GetWidgetLocation() const;
    virtual void ProcessClick(class FSceneView& View, class HHitProxy* HitProxy, FKey Key, EInputEvent Event, uint32 HitX, uint32 HitY) override;
    virtual void Tick(float InDeltaTime) override;
    virtual void TrackingStopped() override;
    virtual bool InputKey(const FInputKeyEventArgs& EventArgs) override;
    virtual bool InputWidgetDelta(FViewport* InViewport, EAxisList::Type CurrentAxis, FVector& Drag, FRotator& Rot, FVector& Scale) override;

    virtual  FSceneView* CalcSceneView(FSceneViewFamily* ViewFamily, const int32 StereoViewIndex = INDEX_NONE) override;
    // TODO: Deprecate
    bool HandleMousePress(const FInputKeyEventArgs& EventArgs);
    bool HandleMouseClick(const FInputKeyEventArgs& EventArgs);
    bool HandleMouseRelease(const FInputKeyEventArgs& EventArgs);
    // TODO: Deprecate 物体选择和变换相关方法
    void SelectActor(AActor* Actor, USceneComponent* Component);
    void ClearSelection();
    void UpdateGizmoTransform();
    bool DeprojectScreenToWorld(const FVector2D& ScreenPosition, FVector& OutWorldOrigin, FVector& OutWorldDirection);
    void SaveCurrentSelectUObjectData();

    // 获取当前选中的Actor
    AActor* GetSelectedActor() const { return SelectedActor; }

    FORCEINLINE bool GetInputMode() { return bIsFocusHUD; }

    FORCEINLINE void SetInputMode(bool bFocusHUD = false) { bIsFocusHUD = bFocusHUD; }

    FORCEINLINE FWidget* GetWidget() { return Widget; }

    FORCEINLINE TObjectPtr<UTypedElementViewportInteraction> GetViewportInteraction() { return ViewportInteraction; }
};
