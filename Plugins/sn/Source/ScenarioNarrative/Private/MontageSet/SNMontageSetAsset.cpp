#include "MontageSet/SNMontageSetAsset.h"

#define LOCTEXT_NAMESPACE "USNMontageSetAsset"

USNAnimationClipData* USNMontageSetAsset::CreateNewClip()
{
    USNAnimationClipData* NewClip = NewObject<USNAnimationClipData>(this, USNAnimationClipData::StaticClass(), NAME_None, RF_Transactional);
    if (Animation)
    {
        NewClip->EndFrame = Animation->GetNumberOfSampledKeys() - 1;
    }
    Clips.Add(NewClip);
    return NewClip;
}

USNAnimationClipData* USNMontageSetAsset::GetClip(const FString& InName) const
{
    const auto& FindRes = Clips.FindByPredicate([&InName](const TObjectPtr<USNAnimationClipData>& Item)
    {
        return Item->Name == InName;
    });

    if (!FindRes)
    {
        return nullptr;
    }

    return FindRes->Get();
}

TArray<UAnimMontage> USNMontageSetAsset::CreateMontages() const
{
    // TODO: create montage in zzaction preload stage
    UE_LOG(LogTemp, Error, TEXT("Create Montage Not Implement Yet."));
    return {};
}

#undef LOCTEXT_NAMESPACE
