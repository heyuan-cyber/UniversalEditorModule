#pragma once

class USNAssetBase;
class FSNAssetPreviewBase;

class SCENARIONARRATIVEEDITOR_API FSNAssetPreviewRegistry
{
public:
    FSNAssetPreviewRegistry() = default;

    static FSNAssetPreviewRegistry& Get();

    void Register(const UClass* AssetClass, FSNAssetPreviewBase* Resource);
    FSNAssetPreviewBase* GetResource(const USNAssetBase* Asset) const;

    void Clear();

private:
    TMap<const UClass*, FSNAssetPreviewBase*> RegisteredResource;
};
