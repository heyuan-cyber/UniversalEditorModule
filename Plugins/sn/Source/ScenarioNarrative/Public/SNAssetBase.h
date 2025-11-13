#pragma once

#include "SNAssetBase.generated.h"

UCLASS(Abstract, BlueprintType)
class SCENARIONARRATIVE_API USNAssetBase : public UObject
{
    GENERATED_BODY()

public:
    USNAssetBase();

    UPROPERTY(EditInstanceOnly, Category="Generic")
    FString AssetID;

    UFUNCTION()
    virtual bool Validate() const { return true; }

    virtual TSharedRef<FJsonObject> ToJsonObject() const;
};
