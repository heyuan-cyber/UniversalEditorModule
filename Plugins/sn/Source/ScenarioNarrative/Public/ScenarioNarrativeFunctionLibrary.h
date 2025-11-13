// ScenarioNarrativeFunctionLibrary.h
// @author : xiangyp
// @date : 2025/05/22 10:07:52
// @desc : 功能函数库

#pragma once

class SCENARIONARRATIVE_API FScenarioNarrativeFunctionLibrary
{
public:
    static float GetAnimationTimeAtFrame(const UAnimSequenceBase* Animation, int32 Frame);
    static UObject* CreateTransientUObject(UClass* ObjectClass);

    template<typename T>
    static T* CreateTransientObject()
    {
        return NewObject<T>(
            GetTransientPackage(),
            T::StaticClass(),
            NAME_None,
            RF_Transient
        );
    }
};
