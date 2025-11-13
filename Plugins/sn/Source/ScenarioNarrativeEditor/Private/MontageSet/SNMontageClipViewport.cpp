#include "MontageSet/SNMontageClipViewport.h"

#include "Animation/DebugSkelMeshComponent.h"
#include "MontageSet/SNMontageSetPreviewScene.h"
#include "MontageSet/SNMontageSetViewportClient.h"

void SSNMontageClipViewport::Construct(const FArguments& InArgs,
                                     TSharedPtr<FSNMontageSetEditorToolkit> InAssetEditor,
                                     TSharedPtr<FSNMontageSetPreviewScene> InPreviewScene)
{
    EditorToolkit = InAssetEditor;
    PreviewScene = InPreviewScene;
    SEditorViewport::Construct(SEditorViewport::FArguments());
}

SSNMontageClipViewport::~SSNMontageClipViewport()
{
    if (!ViewportClient.IsValid())
    {
        return;
    }

    ViewportClient->ViewportWidget = nullptr;
}

void SSNMontageClipViewport::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime,
                                const float InDeltaTime)
{
    SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
}

void SSNMontageClipViewport::AddReferencedObjects(FReferenceCollector& Collector)
{
}

FString SSNMontageClipViewport::GetReferencerName() const
{
    return TEXT("AnimationClipViewport");
}

TSharedPtr<FExtender> SSNMontageClipViewport::GetExtenders() const
{
    return MakeShared<FExtender>();
}

void SSNMontageClipViewport::OnFloatingButtonClicked()
{
}

void SSNMontageClipViewport::OnFocusViewportToSelection()
{
    SEditorViewport::OnFocusViewportToSelection();
    ViewportClient->SetViewRotation(FRotator(0, -90, 0));
    if (PreviewScene->GetPreviewSkeletonMesh())
    {
        ViewportClient->FocusViewportOnBounds(PreviewScene->GetPreviewSkeletonMesh()->Bounds);
    }
}

TSharedRef<FEditorViewportClient> SSNMontageClipViewport::MakeEditorViewportClient()
{
    ViewportClient = MakeShareable(new FSNMontageSetViewportClient(SharedThis(this), PreviewScene.ToSharedRef()));
    return ViewportClient.ToSharedRef();
}

TSharedPtr<FSNMontageSetViewportClient> SSNMontageClipViewport::GetViewportClient() const
{
    return ViewportClient;
}

TSharedRef<SEditorViewport> SSNMontageClipViewport::GetViewportWidget()
{
    return SharedThis(this);
}
