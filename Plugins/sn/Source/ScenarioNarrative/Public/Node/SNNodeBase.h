#pragma once

#include "SNAssetBase.h"
#include "SNNodeBase.generated.h"

class FSNSequencerContext;

UCLASS(EditInlineNew, Abstract, BlueprintType)
class SCENARIONARRATIVE_API USNNodeBase : public USNAssetBase
{
    GENERATED_BODY()

public:
    USNNodeBase() = default;
    virtual ~USNNodeBase() override = default;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic", meta=(EditCondition = "bCanRename", EditConditionHides, JsonIgnore))
    FName DisplayName;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Generic")
    float BeginTime = 0;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Generic")
    float EndTime = 1.f;

    UPROPERTY(BlueprintReadOnly, meta=(JsonIgnore))
    TObjectPtr<class USNTrackBase> Parent;

#if WITH_EDITORONLY_DATA

    UPROPERTY(meta=(JsonIgnore))
    bool bIsValid = true;

    UPROPERTY(meta=(JsonIgnore))
    bool bResizable = true;

    UPROPERTY(meta=(JsonIgnore))
    bool bCanRename = true;

#endif

    UFUNCTION()
    float Length() const;

    UFUNCTION()
    bool CanAttachTo(const UClass* TargetTrack) const;

    // include TrackBase means can attach to any type of track
    virtual TSet<UClass*> AttachedToTracks() const;

    UFUNCTION()
    virtual float GetMaxLength() const;
    UFUNCTION()
    virtual float GetMinLength() const;

    virtual bool Validate() const override;

    virtual TSharedPtr<FJsonObject> NodeDataToJson();
};
