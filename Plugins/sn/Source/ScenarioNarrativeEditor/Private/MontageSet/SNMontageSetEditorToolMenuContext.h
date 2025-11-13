#pragma once

#include "UObject/Object.h"
#include "SNMontageSetEditorToolMenuContext.generated.h"

UCLASS()
class USNMontageSetEditorToolMenuContext : public UObject
{
    GENERATED_BODY()

public:
    static void OnCreateMontageSetClicked(const TArray<UAnimSequence*>& InAssets);
};
