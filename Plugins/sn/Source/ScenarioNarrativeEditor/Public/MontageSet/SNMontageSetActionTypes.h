#pragma once

#include "CoreMinimal.h"
#include "MontageSet/SNMontageSetAsset.h"
#include "AssetTypeActions_Base.h"

class SCENARIONARRATIVEEDITOR_API FSNMontageSetTypeActions : public FAssetTypeActions_Base
{
public:
    virtual UClass* GetSupportedClass() const override { return USNMontageSetAsset::StaticClass(); }
    virtual FText GetName() const override { return INVTEXT("SN Montage Set"); }
    virtual FColor GetTypeColor() const override { return FColor::Silver; }
    virtual uint32 GetCategories() override { return EAssetTypeCategories::Animation; }

    // virtual const FSlateBrush* GetIconBrush(const FAssetData& InAssetData, const FName InClassName) const override;

    virtual void OpenAssetEditor(const TArray<UObject*>& InObjects,
                                 TSharedPtr<IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;

    virtual const FSlateBrush* GetThumbnailBrush(const FAssetData& InAssetData, const FName InClassName) const override;
};
