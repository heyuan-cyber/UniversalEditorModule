#pragma once

#include "SNAssetBase.h"
#include "SNTrackBase.generated.h"

class USNNodeBase;

UCLASS(EditInlineNew, Abstract, BlueprintType)
class SCENARIONARRATIVE_API USNTrackBase : public USNAssetBase
{
    GENERATED_BODY()

public:
    USNTrackBase() = default;
    virtual ~USNTrackBase() override = default;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic", meta=(JsonIgnore))
    FName DisplayName;

    UPROPERTY(BlueprintReadOnly, Instanced, meta=(JsonIgnore))
    TArray<TObjectPtr<USNNodeBase>> Nodes;

    TObjectPtr<USNNodeBase> MakeNewNode(const UClass* Class, const FName& InDisplayName);

    template <typename T = USNNodeBase>
    TObjectPtr<T> MakeNewNode(const FName& InDisplayName);

    bool AddNode(const TObjectPtr<USNNodeBase>& Node);
    bool RemoveNode(const TObjectPtr<USNNodeBase>& Node);

    UFUNCTION()
    float GetLength() const;

    virtual TSharedPtr<FJsonObject> TrackDataToJson() const;
};
