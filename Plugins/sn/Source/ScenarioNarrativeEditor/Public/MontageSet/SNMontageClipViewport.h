#pragma once

#include "CoreMinimal.h"
#include "SCommonEditorViewportToolbarBase.h"
#include "SEditorViewport.h"

class USNMontageSetAsset;
class FSNMontageSetPreviewScene;
class FSNMontageSetEditorToolkit;
class FSNMontageSetViewportClient;

class SSNMontageClipViewport : public SEditorViewport, public FGCObject, public ICommonEditorViewportToolbarInfoProvider
{
public:
    SLATE_BEGIN_ARGS(SSNMontageClipViewport)
        {
        }

    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, TSharedPtr<FSNMontageSetEditorToolkit> InAssetEditor,
                   TSharedPtr<FSNMontageSetPreviewScene> InPreviewScene);
    virtual ~SSNMontageClipViewport() override;

    virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
    virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

    virtual FString GetReferencerName() const override;
    virtual TSharedRef<SEditorViewport> GetViewportWidget() override;
    virtual TSharedPtr<FExtender> GetExtenders() const override;
    virtual void OnFloatingButtonClicked() override;
    virtual void OnFocusViewportToSelection() override;

    virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;
    TSharedPtr<FSNMontageSetViewportClient> GetViewportClient() const;

private:
    TSharedPtr<FSNMontageSetPreviewScene> PreviewScene;
    TSharedPtr<FSNMontageSetViewportClient> ViewportClient;
    TSharedPtr<FSNMontageSetEditorToolkit> EditorToolkit;
};
