#include "MontageSet/SNMontageSetModel.h"
#include "MontageSet/SNMontageSetAsset.h"

FSNMontageSetModel::FSNMontageSetModel(USNMontageSetAsset* InAsset)
    : Asset(MakeShareable(InAsset))
{
}

FSNMontageSetModel::~FSNMontageSetModel()
{
}

const USNMontageSetAsset* FSNMontageSetModel::GetAsset() const
{
    if (Asset.IsValid())
    {
        return Asset.Get();
    }

    return nullptr;
}
