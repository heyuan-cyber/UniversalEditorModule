#include "DataStyle/Node/SNMontageSetNodeStyle.h"

#include "Node/SNMontageSetNode.h"

FSNMontageSetNodeStyle::FSNMontageSetNodeStyle()
{
    BottomColor = FColor::Silver;
}

FString FSNMontageSetNodeStyle::GetTypeName() const
{
    return TEXT("动画切片节点");
}

FText FSNMontageSetNodeStyle::GetTooltipText(const TObjectPtr<USNAssetBase>& Asset) const
{
    if (!Asset)
    {
        return {};
    }

    const auto& MontageSetAsset = Cast<USNMontageSetNode>(Asset);
    return FText::FromName(MontageSetAsset->DisplayName);
}
