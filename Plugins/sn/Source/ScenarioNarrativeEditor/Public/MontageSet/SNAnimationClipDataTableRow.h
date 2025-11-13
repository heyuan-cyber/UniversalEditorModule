#pragma once

#include "CoreMinimal.h"
#include "ScenarioNarrativeEditorDefines.h"

class USNMontageSetAsset;
class USNAnimationClipData;

class SCENARIONARRATIVEEDITOR_API SSNAnimationClipDataTableRow : public SMultiColumnTableRow<FSNClipDataPtr>
{
    SLATE_BEGIN_ARGS(SSNAnimationClipDataTableRow)
        {
        }

        SLATE_ARGUMENT(FSNClipDataPtr, Clip)
        SLATE_ARGUMENT(TArray<FSNClipDataPtr>*, ClipDatas)
        SLATE_ARGUMENT(TSharedPtr<SListView<FSNClipDataPtr>>, ListView)
        SLATE_ARGUMENT(TWeakObjectPtr<USNMontageSetAsset>, Asset)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& OwnerTable);

    virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override;

    virtual FReply OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent) override;

private:
    FSNClipDataPtr Item;
    TSharedPtr<SInlineEditableTextBlock> NameTextBlock;
    TArray<FSNClipDataPtr>* Datas = nullptr;
    TSharedPtr<SListView<FSNClipDataPtr>> ListView;
    TWeakObjectPtr<USNMontageSetAsset> Asset;
};
