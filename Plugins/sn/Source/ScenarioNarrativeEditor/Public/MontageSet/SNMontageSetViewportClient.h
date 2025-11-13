#pragma once

#include "CoreMinimal.h"
#include "AdvancedPreviewScene.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "EditorViewportClient.h"
#include "SEditorViewport.h"

class FSNMontageSetPreviewScene;
class FSNMontageSetEditorToolkit;
class SMontageClipViewport;

class FSNMontageSetViewportClient : public FEditorViewportClient
{
public:
    TWeakPtr<SEditorViewport> ViewportWidget;

    FSNMontageSetViewportClient(const TSharedRef<SEditorViewport>& InThumbnailViewport, const TSharedRef<FAdvancedPreviewScene>& InPreviewScene);

    void FocusViewportOnBounds(const FBoxSphereBounds& Bounds, bool bInstant = false);

private:
    TSharedPtr<FSNMontageSetPreviewScene> PreviewScene;
};
