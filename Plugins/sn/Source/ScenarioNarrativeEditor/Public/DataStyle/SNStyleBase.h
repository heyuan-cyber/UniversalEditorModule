// SNAssetStyleBase.h
// @author : xiangyp
// @date : 2025/04/22 15:57:52
// @desc :

#pragma once
#include "CoreMinimal.h"
#include "SNAssetBase.h"
#include "SNAssetStyleRegistry.h"

class FSNSequencerContext;
class USNAssetBase;

#define AUTO_REGISTER_STYLE(AssetClass, StyleClass) \
static TStyleAutoRegister<StyleClass> StyleClass##_AutoReg(AssetClass::StaticClass());

template<typename T>
struct TStyleAutoRegister
{
    TStyleAutoRegister(UClass* AssetClass)
    {
        FSNAssetStyleRegistry::Get().RegisteredStyle.Add(AssetClass, std::move(new T()));
    }
};



class FSNAssetStyleBase : public TSharedFromThis<FSNAssetStyleBase>
{
public:
    FSNAssetStyleBase() = default;
    // FSNAssetStyleBase(const FSNAssetStyleBase& Other) = default;
    virtual ~FSNAssetStyleBase() = default;

    virtual FText GetTooltipText(const TObjectPtr<USNAssetBase>& Asset) const { return {}; }

    virtual FString GetTypeName() const { return {}; }

    virtual void OnCreateContextMenu(FMenuBuilder& MenuBuilder, const USNAssetBase* Asset, FSNSequencerContext* Context);

};
