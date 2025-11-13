#pragma once

#include "CoreMinimal.h"
#include "ScenarioNarrativeEditorDefines.h"

class USNMontageSetAsset;
class USNAnimationClipData;

class SSNMontageSetClipWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SSNMontageSetClipWidget)
    {
    }

    SLATE_ATTRIBUTE(TObjectPtr<USNMontageSetAsset>, Asset)

    SLATE_EVENT(FOnAnimationChanged, OnAnimationChanged)
    SLATE_EVENT(FOnSelctedClipChanged, OnSelectedClipChanged)

SLATE_END_ARGS()

void Construct(const FArguments& InArgs);
    virtual ~SSNMontageSetClipWidget() override = default;

    FSNClipDataPtr GetSelectedClipData() const { return CurrentSelected; }
    void RequestListRefresh() const;

private:
    TWeakObjectPtr<USNMontageSetAsset> Asset;
    FSNClipDataPtr CurrentSelected;

    TSharedPtr<SListView<FSNClipDataPtr>> ClipsListView;
    TArray<FSNClipDataPtr> ClipDatas;

    FOnAnimationChanged OnAnimationChanged;
    FOnSelctedClipChanged OnSelctedClipChanged;

    void SetAsset(USNMontageSetAsset* InAsset);

    TArray<FSNClipDataPtr> GetClips() const;
    TSharedRef<ITableRow> GenerateClipRow(FSNClipDataPtr Item, const TSharedRef<STableViewBase>& Owner);
    TSharedRef<SHorizontalBox> MakeAnimationAssetSelectorWidget() const;
    TSharedRef<SVerticalBox> MakeAssetControlWidget();
    TSharedRef<SListView<FSNClipDataPtr>> MakeClipListView();
    void GenerateClipSequence(const FSNClipDataPtr& Clip) const;
    // void TrimSequences(UAnimSequence* AnimationSequence, int32 StartFrame, int32 EndFrame);
    void TryRelink() const;
};
