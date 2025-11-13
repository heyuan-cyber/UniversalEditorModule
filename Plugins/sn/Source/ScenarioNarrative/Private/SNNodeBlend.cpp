#include "SNNodeBlend.h"

FSNNodeBlend::FSNNodeBlend()
{
    BlendTime = 0.2f;
    BlendOption = EAlphaBlendOption::Linear;
    CustomCurve = nullptr;
    bIsReverse = false;
}

FSNNodeBlend::FSNNodeBlend(float InBlendTime)
{
    BlendTime = InBlendTime;
    BlendOption = EAlphaBlendOption::Linear;
    CustomCurve = nullptr;
    bIsReverse = false;
}

FSNNodeBlend::FSNNodeBlend(const struct FSNNodeBlend& InBlend)
{
    BlendTime = InBlend.BlendTime;
    BlendOption = InBlend.BlendOption;
    CustomCurve = InBlend.CustomCurve;
    bIsReverse = InBlend.bIsReverse;
}

float FSNNodeBlend::GetBlendValue(float InTime, const float DefaultValue) const
{
    if (InTime < 0 || InTime > BlendTime)
    {
        return DefaultValue;
    }

    float Alpha = bIsReverse ? (1.f - InTime / BlendTime) : InTime / BlendTime;
    return FAlphaBlend::AlphaToBlendOption(Alpha, BlendOption, CustomCurve);
}
