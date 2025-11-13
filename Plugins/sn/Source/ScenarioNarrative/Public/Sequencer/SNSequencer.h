// SNSequencer.h
// @author : xiangyp
// @date : 2025/04/22 15:22:53
// @desc : 技能数据文件

#pragma once

#include "CoreMinimal.h"
#include "SNAssetBase.h"
#include "SNSequencer.generated.h"

class USNTrackBase;

UCLASS(BlueprintType)
class SCENARIONARRATIVE_API USNSequencer : public USNAssetBase
{
    GENERATED_BODY()

public:
    USNSequencer();
    virtual ~USNSequencer() override = default;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Instanced)
    TArray<TObjectPtr<USNTrackBase>> Tracks;

    TObjectPtr<USNTrackBase> MakeNewTrack(const UClass* Cls, const FName& InDisplayName);

    template <typename T = USNTrackBase>
    TObjectPtr<T> MakeNewTrack(const FName& InDisplayName);

    UFUNCTION()
    float GetMaxLength() const;

    virtual TSharedRef<FJsonObject> ToJsonObject() const override;
};
