#include "SNAssetViewportClient.h"
#include "SNAssetViewport.h"
#include "EditorModeManager.h"
#include "UnrealWidget.h"
#include "Node/SNCameraNode.h"
#include "Preview/SNAssetPreviewCamera.h"
#include "Preview/SNPreviewCamera.h"
#include "PreviewScene/SNAssetPreviewScene.h"
#include "HitProxies.h"
#include "EditorEngine/SNEditorEngine.h"
#include "Editor/UnrealEdEngine.h"
#include "EditorEngine/EditorMode/SNEditorMode.h"


#define LOCTEXT_NAMESPACE "SSNAssetViewportClient"

FSNAssetViewportClient::FSNAssetViewportClient(TSharedPtr<FEditorModeTools> InModeTool, const TSharedRef<SSNAssetViewport>& InThumbnailViewport, const TSharedRef<FSNAssetPreviewScene>& InPreviewScene)
    : FEditorViewportClient(InModeTool.Get(), &InPreviewScene.Get(), StaticCastSharedRef<SEditorViewport>(InThumbnailViewport)),
      SNViewport(InThumbnailViewport)
{
    SetRealtime(true);

    DrawHelper.bDrawGrid = true;
    DrawHelper.bDrawPivot = true;
    DrawHelper.AxesLineThickness = 5.f;
    DrawHelper.PivotSize = 5.f;

    SetViewLocation(FVector(-10.f, 0.f, 5.f));
    SetViewRotation(FVector::Zero().Rotation());

    EngineShowFlags.SetScreenPercentage(true);

    FEditorViewportClient::SetViewportType(LVT_Perspective);
    SetViewModes(VMI_Lit, VMI_Lit);

    // Gizmo Widget
    {
        // 设置自定义Mode
        ModeTools->SetDefaultMode(USNEditorMode::EdModeId);
        // 激活 Mode
        ModeTools->ActivateMode(USNEditorMode::EdModeId);

        VisibilityDelegate.Unbind();
        VisibilityDelegate.BindLambda([this]()
       {
           return true;
       });
    }
}

FSNAssetViewportClient::~FSNAssetViewportClient()
{
    SNEE->Destory();
    SNEE = nullptr;
}

void FSNAssetViewportClient::Draw(const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
    if (!View) return;

    FEditorViewportClient::Draw(View, PDI);
    SNEE->DrawComponentVisualizers(View, PDI);
}

FVector FSNAssetViewportClient::GetWidgetLocation() const
{
    FVector ComponentVisWidgetLocation;
    if (SNEE->CVM->GetWidgetLocation(this, ComponentVisWidgetLocation))
    {
        return ComponentVisWidgetLocation;
    }

    return FEditorViewportClient::GetWidgetLocation();
}

void FSNAssetViewportClient::ProcessClick(FSceneView& View, HHitProxy* HitProxy, FKey Key, EInputEvent Event,
    uint32 HitX, uint32 HitY)
{
    const FViewportClick Click(&View, this, Key, Event, HitX, HitY);

    FEditorViewportClient::ProcessClick(View, HitProxy, Key, Event, HitX, HitY);

    // 选中处理
    {
        if (Click.GetKey() == EKeys::MiddleMouseButton && !Click.IsAltDown() && !Click.IsShiftDown())
        {
            SNEE->ClickViewport(this, Click);
            return;
        }
        if (!ModeTools->HandleClick(this, HitProxy, Click))
        {
            const FTypedElementHandle HitElement = HitProxy ? HitProxy->GetElementHandle() : FTypedElementHandle();

            if (HitProxy == NULL)
            {
                SNEE->ClickBackdrop(this, Click);
            }
            else if (HitProxy->IsA(HWidgetAxis::StaticGetType()))
            {
                const bool bOldModeWidgets1 = EngineShowFlags.ModeWidgets;
                const bool bOldModeWidgets2 = View.Family->EngineShowFlags.ModeWidgets;

                EngineShowFlags.SetModeWidgets(false);
                FSceneViewFamily* SceneViewFamily = const_cast<FSceneViewFamily*>(View.Family);
                SceneViewFamily->EngineShowFlags.SetModeWidgets(false);
                bool bWasWidgetDragging = Widget->IsDragging();
                Widget->SetDragging(false);
                Viewport->InvalidateHitProxy();

                // This will actually re-render the viewport's hit proxies!
                HHitProxy* HitProxyWithoutAxisWidgets = Viewport->GetHitProxy(HitX, HitY);
                if (HitProxyWithoutAxisWidgets != NULL && !HitProxyWithoutAxisWidgets->IsA(HWidgetAxis::StaticGetType()))
                {
                    // Try this again, but without the widget this time!
                    ProcessClick(View, HitProxyWithoutAxisWidgets, Key, Event, HitX, HitY);
                }

                // Undo the evil
                EngineShowFlags.SetModeWidgets(bOldModeWidgets1);
                SceneViewFamily->EngineShowFlags.SetModeWidgets(bOldModeWidgets2);

                Widget->SetDragging(bWasWidgetDragging);

                Viewport->InvalidateHitProxy();
            }
            else if (SNEE->CVM->HandleClick(this, HitProxy, Click))
            {
            }
            else if (HitElement && SNEE->ClickElement(this, HitElement, Click))
            {
            }
        }
    }

}

bool FSNAssetViewportClient::InputKey(const FInputKeyEventArgs& InEventArgs)
{
    // Rout to Component
    if (SNEE->CVM->HandleInputKey(this, InEventArgs.Viewport, InEventArgs.Key, InEventArgs.Event))
    {
        return true;
    }

    bool bCtrlDown = InEventArgs.Viewport->KeyState(EKeys::LeftControl) || InEventArgs.Viewport->KeyState(EKeys::RightControl);

    if (bCtrlDown)
    {
        UWorld* EditorWorld = GEditor->GetEditorWorldContext().World();
        float CurrentTime = EditorWorld->GetTimeSeconds();
        if (InEventArgs.Key == EKeys::Z)
        {
            if (FMath::Abs(CurrentTime - LastExecTransactionTime) < 0.05f) { return true; }
            GEditor->UndoTransaction();
            LastExecTransactionTime = CurrentTime;
            return true;
        }
        if (InEventArgs.Key == EKeys::Y)
        {
            if (FMath::Abs(CurrentTime -LastExecTransactionTime) < 0.05f) { return true; }
            GEditor->RedoTransaction();
            LastExecTransactionTime = CurrentTime;
            return true;
        }
        if (InEventArgs.Key == EKeys::R)
        {
            if (FMath::Abs(CurrentTime -LastExecTransactionTime) < 0.05f) { return true; }
            TransformType = TransformType == ETransformType::World? ETransformType::Relative : ETransformType::World;
            LastExecTransactionTime = CurrentTime;
            return true;
        }
    }

    // 处理键盘输入（变换模式切换等）
    if (InEventArgs.Event == IE_Pressed && !InEventArgs.Viewport->KeyState(EKeys::RightMouseButton))
    {
        if (InEventArgs.Key == EKeys::W) { ModeTools->SetWidgetMode(UE::Widget::WM_Translate); return true; }
        if (InEventArgs.Key == EKeys::E) { ModeTools->SetWidgetMode(UE::Widget::WM_Rotate);    return true; }
        if (InEventArgs.Key == EKeys::R) { ModeTools->SetWidgetMode(UE::Widget::WM_Scale);     return true; }
        if (InEventArgs.Key == EKeys::Escape) { return true;}
        if (InEventArgs.Key == EKeys::H)      { bUseCustomCamera = !bUseCustomCamera; return true;}
        if (InEventArgs.Key == EKeys::S)      { SaveCurrentSelectUObjectData(); return true;}
    }

     const FModifierKeysState& Modifiers = FSlateApplication::Get().GetModifierKeys();
     const bool bIsControlDown = Modifiers.IsControlDown();
     const bool bIsShiftDown = Modifiers.IsShiftDown();
     if (InEventArgs.Key == EKeys::S && bIsControlDown && bIsShiftDown)
     {
         SaveCurrentSelectUObjectData();
         return true;
     }

    if(GetInputMode()) return true;

    FEditorViewportClient::InputKey(InEventArgs);
    return true;
}

void FSNAssetViewportClient::Tick(float DeltaSeconds)
{
    FEditorViewportClient::Tick(DeltaSeconds);

    // Tick World
    // @TODO : 抽到EditMode中，即插即用
    if (PreviewScene && PreviewScene->GetWorld())
    {
        UWorld* World = PreviewScene->GetWorld();
        World->Tick(LEVELTICK_All, DeltaSeconds);
        World->SendAllEndOfFrameUpdates();
    }
}

void FSNAssetViewportClient::TrackingStopped()
{
    if (AActor* MovedActor = SNEE->GetSelectedActor())
    {
        MovedActor->PostEditMove(true);
    }
}

bool FSNAssetViewportClient::InputWidgetDelta(FViewport* InViewport, EAxisList::Type CurrentAxis, FVector& Drag,
    FRotator& Rot, FVector& Scale)
{
    if (SNEE->CVM->HandleInputDelta(this, InViewport, Drag, Rot, Scale))
    {
        return true;
    }
    bool bHandled = false;
    if (FEditorViewportClient::InputWidgetDelta(InViewport, CurrentAxis, Drag, Rot, Scale))
    {
        bHandled = true;
    }
    else if (CurrentAxis != EAxisList::None)
    {
        // 更新选中的Element的 Transform，没有区分Actor和ActorComponent
        SNEE->ApplyDeltaToSelectedElements(FTransform(Rot, Drag, Scale));
        ApplyDeltaToRotateWidget(Rot);
        ModeTools->PivotLocation += Drag;
        ModeTools->SnappedLocation += Drag;
        ModeTools->UpdateInternalData();
        bHandled = true;
    }
    return bHandled;
}

FSceneView* FSNAssetViewportClient::CalcSceneView(FSceneViewFamily* ViewFamily, const int32 StereoViewIndex)
{
    FSceneView* View = FEditorViewportClient::CalcSceneView(ViewFamily, StereoViewIndex);

    if (bUseCustomCamera)
    {
        TObjectPtr<UCameraComponent> CameraComponent = SNViewport.Pin()->GetPreviewCamera()->CameraComponent;
        if (CameraComponent->IsValidLowLevel())
        {
            FMinimalViewInfo CameraViewInfo;
            CameraComponent->GetCameraView(_DeltaSeconds, CameraViewInfo);
            View->UpdateProjectionMatrix(CameraViewInfo.CalculateProjectionMatrix());
            View->FOV = CameraViewInfo.FOV;
            View->FinalPostProcessSettings = FFinalPostProcessSettings();
            View->OverridePostProcessSettings(CameraViewInfo.PostProcessSettings, CameraViewInfo.PostProcessBlendWeight);

            SetViewLocation(CameraViewInfo.Location);
            SetViewRotation(CameraViewInfo.Rotation);
        }
    }
    return View;
}

bool FSNAssetViewportClient::DeprojectScreenToWorld(const FVector2D& ScreenPosition, FVector& OutWorldOrigin, FVector& OutWorldDirection)
{
    // 方法1: 从 FModeTools 获取
    if (GEditor)
    {
        FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(
            this->Viewport,
            this->GetScene(),
            this->EngineShowFlags).SetRealtimeUpdate(this->IsRealtime()));
        FSceneView* View = this->CalcSceneView(&ViewFamily);
        FViewportCursorLocation MouseViewportRay(View, this, ScreenPosition.X, ScreenPosition.Y);

        FVector RayOrigin = MouseViewportRay.GetOrigin();
        FVector RayDirection = MouseViewportRay.GetDirection();
        if (this->IsOrtho())
        {
            RayOrigin -= 0.1 * HALF_WORLD_MAX * RayDirection;
        }
        OutWorldOrigin = RayOrigin;
        OutWorldDirection = RayDirection;

        return true;
    }

    // 如果无法获取 EditorInteractiveToolsContext 或方法失败，使用备选方法
    UE_LOG(LogTemp, Warning, TEXT("无法获取 UEditorInteractiveToolsContext，使用备选方法"));

    // 备选方法：使用简化的计算
    FVector CameraLocation = GetViewLocation();
    FRotator CameraRotation = GetViewRotation();

    OutWorldOrigin = CameraLocation;
    OutWorldDirection = CameraRotation.Vector();

    return true;
}

void FSNAssetViewportClient::SaveCurrentSelectUObjectData()
{
    UObject* CurrentSelectObject = SNViewport.Pin()->GetCurrentSelectedUObject();
    AActor* Selected = SNEE->GetSelectedActor();
    if(CurrentSelectObject != nullptr && Selected)
    {
        ASNPreviewCamera* SNPreviewCamera = Cast<ASNPreviewCamera>(Selected);
        if(Cast<USNCameraKeyFrameWrapper>(CurrentSelectObject) && SNPreviewCamera)
        {
            USNCameraKeyFrameWrapper* SNCameraKeyFrameWrapper = Cast<USNCameraKeyFrameWrapper>(CurrentSelectObject);
            int32 index = SNCameraKeyFrameWrapper->KeyFrameId;
            SNCameraKeyFrameWrapper->SourceNode->KeyFrames[index].Position = SNPreviewCamera->GetActorLocation();
            SNCameraKeyFrameWrapper->SourceNode->KeyFrames[index].Rotation = SNPreviewCamera->GetActorRotation();
            SNCameraKeyFrameWrapper->CameraKeyFrame = SNCameraKeyFrameWrapper->SourceNode->KeyFrames[index];
        }
    }
}
