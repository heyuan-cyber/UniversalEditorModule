// NodeBlend.h
// @author : xiangyp
// @date : 2025/05/15 13:41:14
// @desc :

#pragma once

#include "SNNodeBlend.generated.h"

USTRUCT(BlueprintType)
struct FSNNodeBlend
{
    GENERATED_BODY()

    SCENARIONARRATIVE_API FSNNodeBlend();
    SCENARIONARRATIVE_API FSNNodeBlend(float InBlendTime);
    SCENARIONARRATIVE_API FSNNodeBlend(const struct FSNNodeBlend& InBlend);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BlendTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EAlphaBlendOption BlendOption;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UCurveFloat> CustomCurve;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsReverse;

    // range [0,1]
    SCENARIONARRATIVE_API float GetBlendValue(float InTime, const float DefaultValue = 1.f) const;
};
