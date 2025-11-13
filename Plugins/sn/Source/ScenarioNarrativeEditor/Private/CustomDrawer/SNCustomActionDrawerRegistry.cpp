#include "CustomDrawer/SNCustomActionDrawerRegistry.h"

#include "SNAssetBase.h"
#include "CustomDrawer/SNCustomActionDrawerBase.h"

FSNCustomActionDrawerRegistry& FSNCustomActionDrawerRegistry::Get()
{
    static TSharedRef<FSNCustomActionDrawerRegistry> GCustomRegistry = MakeShared<FSNCustomActionDrawerRegistry>();
    return GCustomRegistry.Get();
}

void FSNCustomActionDrawerRegistry::Register(const UClass* AssetClass, FSNCustomActionDrawerBase* Drawer)
{
    RegisteredDrawer.Add(AssetClass, Drawer);
}

FSNCustomActionDrawerBase* FSNCustomActionDrawerRegistry::GetDrawer(const USNAssetBase* Asset) const
{
    return RegisteredDrawer.Contains(Asset->GetClass()) ? RegisteredDrawer[Asset->GetClass()] : nullptr;
}

void FSNCustomActionDrawerRegistry::Clear()
{
    for (const auto& Pair : RegisteredDrawer)
    {
        delete Pair.Value;
    }
    RegisteredDrawer.Empty();
}
