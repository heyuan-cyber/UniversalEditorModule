#include "SNDialogNodeStyle.h"
#include "Node/SNDialogNode.h"

#define LOCTEXT_NAMESPACE "FSNDialogNodeStyle"

FSNDialogNodeStyle::FSNDialogNodeStyle()
{
    BottomColor = FLinearColor::FromSRGBColor(FColor(80, 123, 72));
}

FString FSNDialogNodeStyle::GetTypeName() const
{
    return TEXT("对话节点");
}

FText FSNDialogNodeStyle::GetTooltipText(const TObjectPtr<USNAssetBase>& Asset) const
{
    if (!Asset)
    {
        return {};
    }

    return FText::GetEmpty();
}

void FSNDialogNodeStyle::OnContextMenu(FMenuBuilder& MenuBuilder, const USNAssetBase* Asset,
    FSNSequencerContext* Context)
{
    FSNNodeStyle::OnContextMenu(MenuBuilder, Asset, Context);
}


#undef LOCTEXT_NAMESPACE
