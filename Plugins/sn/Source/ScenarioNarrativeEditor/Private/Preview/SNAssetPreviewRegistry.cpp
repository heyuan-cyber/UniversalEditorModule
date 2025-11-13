#include "Preview//SNAssetPreviewRegistry.h"

#include "SNAssetBase.h"
#include "Preview/SNAssetPreviewBase.h"

FSNAssetPreviewRegistry& FSNAssetPreviewRegistry::Get()
{
    static TSharedRef<FSNAssetPreviewRegistry> GCustomRegistry = MakeShared<FSNAssetPreviewRegistry>();
    return GCustomRegistry.Get();
}

void FSNAssetPreviewRegistry::Register(const UClass* AssetClass, FSNAssetPreviewBase* Drawer)
{
    RegisteredResource.Add(AssetClass, Drawer);
}

FSNAssetPreviewBase* FSNAssetPreviewRegistry::GetResource(const USNAssetBase* Asset) const
{
    return RegisteredResource.Contains(Asset->GetClass()) ? RegisteredResource[Asset->GetClass()] : nullptr;
}

void FSNAssetPreviewRegistry::Clear()
{
    for (const auto& Pair : RegisteredResource)
    {
        delete Pair.Value;
    }
    RegisteredResource.Empty();
}
