#include "ScenarioNarrativeFunctionLibrary.h"

#include "ScenarioNarrativeDefines.h"

float FScenarioNarrativeFunctionLibrary::GetAnimationTimeAtFrame(const UAnimSequenceBase* Animation, int32 Frame)
{
    if (!Animation)
        return 0;

    const int32 FrameCount = Animation->GetNumberOfSampledKeys();
    ensureMsgf(FrameCount > 0, TEXT("animation frame count invalid."));

    if (Frame <= 0 || Frame >= FrameCount)
    {
        UE_LOG(LogScenarioNarrative, Error, TEXT("Animation Frame(%d) Invalid!"), Frame);
        return 0;
    }

    float PlayLength = Animation->GetPlayLength();
    const float TimePerKey = PlayLength / FrameCount;

    return Frame * TimePerKey;
}

UObject* FScenarioNarrativeFunctionLibrary::CreateTransientUObject(UClass* ObjectClass)
{
    // 使用 Transient 包创建对象
    UObject* TransientObject = NewObject<UObject>(
        GetTransientPackage(), // 使用临时包
        ObjectClass,           // 对象的类
        NAME_None,             // 不指定名称（自动生成）
        RF_Transient           // 标记为临时对象
    );

    return TransientObject;
}
