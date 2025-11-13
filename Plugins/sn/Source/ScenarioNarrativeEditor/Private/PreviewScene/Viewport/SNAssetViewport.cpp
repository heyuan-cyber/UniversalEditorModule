#include "SNAssetViewport.h"
#include "SNAssetViewportClient.h"
#include "SNSequencerContext.h"
#include "Camera/CameraActor.h"
#include "EditorEngine/SNEditorEngine.h"
#include "Preview/UI/SNCameraPreview.h"
#include "Slate/SceneViewport.h"


void SSNAssetViewport::Construct(const FArguments& InArgs)
{
    PreviewScene = InArgs._PreviewScene;
    Context = InArgs._Context;
    InspectorWidget = InArgs._InspectorWidget;
    InModeTools = InArgs._InModeTools;
    SEditorViewport::Construct(SEditorViewport::FArguments());
}

SSNAssetViewport::~SSNAssetViewport()
{
    if (GetViewportClient().IsValid())
    {
        GetViewportClient().Reset();
    }
    PreviewCamera(nullptr);
    PreviewScene.Reset();
}

TSharedRef<FEditorViewportClient> SSNAssetViewport::MakeEditorViewportClient()
{
    return MakeShared<FSNAssetViewportClient>(InModeTools, StaticCastSharedRef<SSNAssetViewport>(AsShared()),
        PreviewScene.Pin().ToSharedRef());
}

void SSNAssetViewport::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
    SEditorViewport::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
    PreviewCamera(Cast<ACameraActor>(USNEditorEngine::Get()->GetSelectedActor()));
}

void SSNAssetViewport::ShowObjectInspectorWidget(UObject* object) const
{
    // InspectorWidget->SetObject(object);
    Context->SetSelectUObject(object);
}

UObject* SSNAssetViewport::GetCurrentSelectedUObject()
{
    return Context->GetCurrentSelectedUObject();
}

FSNAssetViewportClient* SSNAssetViewport::GetSNViewportClient()
{
    return static_cast<FSNAssetViewportClient*>(Client.Get());
}

void SSNAssetViewport::AddViewportWidget(TSharedPtr<SWidget> Widget)
{
    // 添加到叠加层
    ViewportOverlay->AddSlot(1)
        .HAlign(HAlign_Center)
        .VAlign(VAlign_Bottom)
        .Padding(FMargin(0, 0, 0, 50))
        [
            SNew(SBox)
                .WidthOverride(800)
                .HeightOverride(200)
                [
                    Widget.ToSharedRef()
                ]
        ];
}

FReply SSNAssetViewport::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
    return SEditorViewport::OnKeyDown(MyGeometry, InKeyEvent);
}

void SSNAssetViewport::PreviewCamera(const ACameraActor* InCamera)
{
    if (InCamera && !PreviewingCamera)
    {
        AddCameraPreview(InCamera);
    }
    else if (!InCamera && PreviewingCamera)
    {
        RemoveCameraPreview();
    }
}
void SSNAssetViewport::RemoveCameraPreview()
{
    this->PreviewingCamera = nullptr;
    if (CameraPreviewWidget.IsValid())
    {
        ViewportOverlay->RemoveSlot(CameraPreviewWidget.ToSharedRef());
        CameraPreviewWidget.Reset();
    }

    ActorPreviewSceneViewport.Reset();
    CameraPreviewClient->Viewport = nullptr;
    CameraPreviewClient.Reset();
}

void SSNAssetViewport::AddCameraPreview(const ACameraActor* InCamera)
{
    this->PreviewingCamera = const_cast<ACameraActor*>(InCamera);
    TSharedPtr<SWidget> CustomPreviewContent;
    if (UActorComponent* PreviewComp = InCamera->GetComponentByClass(UCameraComponent::StaticClass()))
    {
        CustomPreviewContent = PreviewComp->GetCustomEditorPreviewWidget();
    }
    const bool bNeedsViewport = !CustomPreviewContent.IsValid();

    if (bNeedsViewport)
    {
        CameraPreviewClient = MakeShared<FCameraPreviewClient>(GetWorld(), (SharedThis(this)));

        CameraPreviewClient->ViewportType = LVT_Perspective;
        CameraPreviewClient->bSetListenerPosition = false; // Preview viewports never be a listener

        // Never draw the axes indicator in these small viewports
        CameraPreviewClient->bDrawAxes = false;

        // Default to "game" show flags for camera previews
        // Still draw selection highlight though
        CameraPreviewClient->EngineShowFlags = FEngineShowFlags(ESFIM_Game);
        CameraPreviewClient->EngineShowFlags.SetSelection(true);
        CameraPreviewClient->LastEngineShowFlags = FEngineShowFlags(ESFIM_Editor);
        if (!CameraPreviewClient)
        {
            CameraPreviewClient->EngineShowFlags.Tonemapper = false;
        }

        // We don't use view modes for preview viewports
        CameraPreviewClient->SetViewMode(VMI_Unknown);

        // User should never be able to interact with this viewport
        CameraPreviewClient->bDisableInput = true;

        // Never allow cinematics to possess these views
        CameraPreviewClient->SetAllowCinematicControl(false);

        // Our preview viewport is always visible if our owning SLevelViewport is visible, so we hook up
        // to the same IsVisible method
        CameraPreviewClient->VisibilityDelegate.BindSP(this, &SSNAssetViewport::IsVisible);

        CameraPreviewClient->UpdateViewForLockedActor();

        // Preview the play world if the current actor is in the play world
        if (InCamera->GetWorld()->IsGameWorld())
        {
            CameraPreviewClient->SetIsSimulateInEditorViewport(true);
        }
    }

    if (CustomPreviewContent.IsValid())
    {
        SAssignNew(CameraPreviewWidget, SCameraPreview)
            .PreviewActor(const_cast<ACameraActor*>(InCamera))
            .Content()
            [
                CustomPreviewContent.ToSharedRef()
            ];
    }
    else
    {
        SAssignNew(CameraPreviewWidget, SCameraPreview)
            .PreviewActor(const_cast<ACameraActor*>(InCamera));
    }


    if (true)
    {
        TSharedPtr<SViewport> ActorPreviewViewportWidget = CameraPreviewWidget->GetViewportWidget();

        ActorPreviewSceneViewport = MakeShared<FSceneViewport>(CameraPreviewClient.Get(), ActorPreviewViewportWidget);
        {
            CameraPreviewClient->Viewport = ActorPreviewSceneViewport.Get();
            if (ensure(ActorPreviewViewportWidget.IsValid()))
            {
                ActorPreviewViewportWidget->SetViewportInterface(ActorPreviewSceneViewport.ToSharedRef());
            }
        }
    }

    {
        ViewportOverlay->AddSlot()
            [
                CameraPreviewWidget.ToSharedRef()
            ];
    }
}
