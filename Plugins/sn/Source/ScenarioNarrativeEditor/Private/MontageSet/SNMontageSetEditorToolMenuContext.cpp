#include "SNMontageSetEditorToolMenuContext.h"

#include "IAssetTools.h"
#include "MontageSet/SNMontageSetAsset.h"
#include "MontageSet/SNMontageSetFactory.h"

void USNMontageSetEditorToolMenuContext::OnCreateMontageSetClicked(const TArray<UAnimSequence*>& InAssets)
{
    IAssetTools::Get().CreateAssetsFrom<UAnimSequence>
    (
        InAssets,
        USNMontageSetAsset::StaticClass(),
        TEXT("_MontageSet"),
        [](UAnimSequence* SourceObject)
        {
            USNMontageSetFactory* Factory = NewObject<USNMontageSetFactory>();
            Factory->SourceObject = SourceObject;
            return Factory;
        }
    );
}
