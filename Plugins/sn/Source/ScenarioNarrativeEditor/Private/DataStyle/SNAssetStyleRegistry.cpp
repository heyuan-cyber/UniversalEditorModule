#include "DataStyle/SNAssetStyleRegistry.h"

#include "DataStyle/SNNodeStyle.h"
#include "DataStyle/SNTrackStyle.h"

FSNNodeStyle* FSNAssetStyleRegistry::DefaultNodeStyle = new FSNNodeStyle();
FSNTrackStyle* FSNAssetStyleRegistry::DefaultTrackStyle = new FSNTrackStyle();

FSNAssetStyleRegistry& FSNAssetStyleRegistry::Get()
{
    static TSharedRef<FSNAssetStyleRegistry> GStyleRegistry = MakeShared<FSNAssetStyleRegistry>();
    return GStyleRegistry.Get();
}

void FSNAssetStyleRegistry::Register(const UClass* SNAssetClass, FSNAssetStyleBase* Style)
{
    if (RegisteredStyle.Contains(SNAssetClass) && RegisteredStyle[SNAssetClass] != Style)
    {
        UE_LOG(LogTemp, Error, TEXT("Class %s already registered to wrapper"), *SNAssetClass->GetName());
        return;
    }

    RegisteredStyle.Add(SNAssetClass, Style);
}

FSNNodeStyle* FSNAssetStyleRegistry::GetNodeStyle(const UClass* NodeClass) const
{
    return RegisteredStyle.Contains(NodeClass)
               ? StaticCast<FSNNodeStyle*>(RegisteredStyle[NodeClass])
               : DefaultNodeStyle;
}

FSNTrackStyle* FSNAssetStyleRegistry::GetTrackStyle(const UClass* TrackClass) const
{
    return RegisteredStyle.Contains(TrackClass)
               ? StaticCast<FSNTrackStyle*>(RegisteredStyle[TrackClass])
               : DefaultTrackStyle;
}

void FSNAssetStyleRegistry::Clear()
{
    for (const auto& Style : RegisteredStyle)
    {
        // free memory
        delete Style.Value;
    }

    RegisteredStyle.Empty();
}

template <class T>
void FSNAssetStyleRegistry::Register(FSNAssetStyleBase* Style)
{
    Register(T::StaticClass(), Style);
}

template <class T>
FSNNodeStyle* FSNAssetStyleRegistry::GetNodeStyle() const
{
    return GetNodeStyle(T::StaticClass());
}

template <class T>
FSNTrackStyle* FSNAssetStyleRegistry::GetTrackStyle() const
{
    return GetTrackStyle(T::StaticClass());
}
