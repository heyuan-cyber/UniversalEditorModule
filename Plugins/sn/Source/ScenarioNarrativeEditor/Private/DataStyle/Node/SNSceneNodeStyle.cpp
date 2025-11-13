#include "SNSceneNodeStyle.h"

#define LOCTEXT_NAMESPACE "FFSNDialogNodeStyle"

FSNSceneNodeStyle::FSNSceneNodeStyle()
{
    BottomColor = FLinearColor::FromSRGBColor(FColor(80, 123, 72));
}

FString FSNSceneNodeStyle::GetTypeName() const
{
    return TEXT("场景节点");
}

FText FSNSceneNodeStyle::GetTooltipText(const TObjectPtr<USNAssetBase>& Asset) const
{
    if (!Asset)
    {
        return {};
    }

    return FText::GetEmpty();
}

void FSNSceneNodeStyle::OnContextMenu(FMenuBuilder& MenuBuilder, const USNAssetBase* Asset,
    FSNSequencerContext* Context)
{
    FSNNodeStyle::OnContextMenu(MenuBuilder, Asset, Context);
}


#undef LOCTEXT_NAMESPACE
