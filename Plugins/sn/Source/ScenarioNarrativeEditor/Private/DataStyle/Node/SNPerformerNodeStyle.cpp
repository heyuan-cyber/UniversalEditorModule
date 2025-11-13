#include "SNPerformerNodeStyle.h"
#include "SNDialogNodeStyle.h"
#include "Node/SNDialogNode.h"

#define LOCTEXT_NAMESPACE "FFSNPerformerNodeStyle"

FSNPerformerNodeStyle::FSNPerformerNodeStyle()
{
    BottomColor = FLinearColor::FromSRGBColor(FColor(80, 123, 72));
}

FString FSNPerformerNodeStyle::GetTypeName() const
{
    return TEXT("演员节点");
}

FText FSNPerformerNodeStyle::GetTooltipText(const TObjectPtr<USNAssetBase>& Asset) const
{
    if (!Asset)
    {
        return {};
    }

    return FText::GetEmpty();
}

void FSNPerformerNodeStyle::OnContextMenu(FMenuBuilder& MenuBuilder, const USNAssetBase* Asset,
    FSNSequencerContext* Context)
{
    FSNNodeStyle::OnContextMenu(MenuBuilder, Asset, Context);
}


#undef LOCTEXT_NAMESPACE
