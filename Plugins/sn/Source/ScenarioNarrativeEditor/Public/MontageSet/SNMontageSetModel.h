#pragma once

#include "CoreMinimal.h"

class USNMontageSetAsset;

class SCENARIONARRATIVEEDITOR_API FSNMontageSetModel : public TSharedFromThis<FSNMontageSetModel>
{
public:
    FSNMontageSetModel(USNMontageSetAsset* InAsset);
    virtual ~FSNMontageSetModel();

    const USNMontageSetAsset* GetAsset() const;

private:
    TSharedPtr<USNMontageSetAsset> Asset;
};
