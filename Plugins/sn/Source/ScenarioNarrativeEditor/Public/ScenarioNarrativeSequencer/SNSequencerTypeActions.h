#pragma once

#include "CoreMinimal.h"
#include "Sequencer/SNSequencer.h"
#include "AssetTypeActions_Base.h"

class SCENARIONARRATIVEEDITOR_API FSNSequencerTypeActions : public FAssetTypeActions_Base
{
public:
    virtual UClass* GetSupportedClass() const override { return USNSequencer::StaticClass(); }
    virtual FText GetName() const override { return INVTEXT("SN Sequencer"); }
    virtual FColor GetTypeColor() const override { return FColor::Orange; }
    virtual uint32 GetCategories() override { return EAssetTypeCategories::Gameplay; }

    virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor) override;

    virtual const FSlateBrush* GetThumbnailBrush(const FAssetData& InAssetData, const FName InClassName) const override;
};
