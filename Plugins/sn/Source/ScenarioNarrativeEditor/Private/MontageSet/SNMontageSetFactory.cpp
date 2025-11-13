#include "MontageSet/SNMontageSetFactory.h"
#include "MontageSet/SNMontageSetAsset.h"

USNMontageSetFactory::USNMontageSetFactory()
    : SourceObject(nullptr)
{
    SupportedClass = USNMontageSetAsset::StaticClass();
    bCreateNew = true;
}

UObject* USNMontageSetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags,
                                              UObject* Context, FFeedbackContext* Warn)
{
    USNMontageSetAsset* Asset = NewObject<USNMontageSetAsset>(InParent, Class, Name, Flags, Context);
    if (SourceObject)
    {
        Asset->Animation = SourceObject;
        auto* Clip = Asset->CreateNewClip();
        Clip->Name = SourceObject->GetName();
    }
    return Asset;
}
