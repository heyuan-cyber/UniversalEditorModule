#include "MontageSet/SNMontageSetActionTypes.h"

#include "SNTypeRegistryHelper.h"
#include "MontageSet/SNMontageSetEditorToolkit.h"

void FSNMontageSetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects,
                                             TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
    MakeShared<FSNMontageSetEditorToolkit>()->Initialize(InObjects, EditWithinLevelEditor);
}

const FSlateBrush* FSNMontageSetTypeActions::GetThumbnailBrush(const FAssetData& InAssetData,
                                                             const FName InClassName) const
{
    return FSNTypeRegistryHelper::Get().GetBrush("ClassThumbnail.MontageSet");
}
