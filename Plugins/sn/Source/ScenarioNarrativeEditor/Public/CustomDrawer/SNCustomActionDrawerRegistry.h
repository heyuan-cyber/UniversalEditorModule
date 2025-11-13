#pragma once

class USNAssetBase;
class FSNCustomActionDrawerBase;

class SCENARIONARRATIVEEDITOR_API FSNCustomActionDrawerRegistry
{
public:
    FSNCustomActionDrawerRegistry() = default;

    static FSNCustomActionDrawerRegistry& Get();

    void Register(const UClass* AssetClass, FSNCustomActionDrawerBase* Drawer);
    FSNCustomActionDrawerBase* GetDrawer(const USNAssetBase* Asset) const;

    void Clear();

private:
    TMap<const UClass*, FSNCustomActionDrawerBase*> RegisteredDrawer;
};
