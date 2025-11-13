#pragma once

#include "CoreMinimal.h"
#include "MontageSet/SNAnimationClipDataTableRow.h"

class FMontageSetEditorToolkit;
class UMontageSetAsset;
class UAnimationClipData;
class SScrubWidget;
class SAssetView;

DECLARE_DELEGATE_TwoParams(FOnScrubBarDrag, int32, float)
DECLARE_DELEGATE_TwoParams(FOnScrubBarCommit, int32, float)

using ClipDataPtr = TWeakObjectPtr<USNAnimationClipData>;

class SCENARIONARRATIVEEDITOR_API SSNAnimTimeRulerWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SSNAnimTimeRulerWidget)
        {
        }

        SLATE_ATTRIBUTE(TObjectPtr<USNMontageSetAsset>, Asset)
        SLATE_ATTRIBUTE(TObjectPtr<UDebugSkelMeshComponent>, PreviewMesh)
        SLATE_ATTRIBUTE(TArray<float>, DraggableBars)

        SLATE_EVENT(FOnScrubBarDrag, OnBarDrag)
        SLATE_EVENT(FOnScrubBarCommit, OnBarCommit)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);
    virtual ~SSNAnimTimeRulerWidget() override = default;

    void SetAsset(USNMontageSetAsset* InAsset);
    void SetClipData(ClipDataPtr InClip);
    void SetPreviewMesh(const TObjectPtr<UDebugSkelMeshComponent>& InMesh);
    uint32 GetNumOfKeys() const;
    float GetSequenceLength() const;

private:
    TWeakObjectPtr<USNMontageSetAsset> Asset;
    ClipDataPtr CurrentSelected;
    TSharedPtr<SScrubWidget> ScrubWidget;

    TWeakObjectPtr<UAnimSingleNodeInstance> AnimSingleNodeInstance;

    TSharedRef<SBorder> MakeTimeRulerWidget(const FArguments& InArgs);
    TSharedRef<SWidget> MakeTransportControls() const;
    TSharedRef<SHorizontalBox> MakeControlWidget() const;
};
