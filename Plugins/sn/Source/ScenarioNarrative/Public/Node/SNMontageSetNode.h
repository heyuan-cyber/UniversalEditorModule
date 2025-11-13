#pragma once

#include "CoreMinimal.h"
#include "SNCrossableNode.h"
#include "SNMontageSetNode.generated.h"

class USNMontageSetAsset;

UCLASS()
class SCENARIONARRATIVE_API USNMontageSetNode : public USNCrossableNode
{
    GENERATED_BODY()

public:
    USNMontageSetNode();
    virtual ~USNMontageSetNode() override = default;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<USNMontageSetAsset> MontageSet;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (GetOptions = "GetAllClipNames"))
    FString ClipName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TargetFrame = -1;

    virtual TSet<UClass*> AttachedToTracks() const override;
    virtual bool Validate() const override;

#if WITH_EDITOR
    virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
    UFUNCTION()
    TArray<FString> GetAllClipNames() const;
};
