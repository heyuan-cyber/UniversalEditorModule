// Fill out your copyright notice in the Description page of Project Settings.


#include "Node/KeyFrame/SNKeyFrame.h"

void ISNKeyFrameInterface::GetKeyIndexRangeOfTime(float InTime, int32& PreKeyIndex, int32& NextKeyIndex)
{
    PreKeyIndex = NextKeyIndex = -1;
    const auto& KeyFrames = GetKeyFrames();
    if (KeyFrames.Num() == 0) return;
    int32 index = 0;
    for(; index < KeyFrames.Num(); ++index)
    {
        if (InTime - KeyFrames[index].LocalTime < 0)
        {
            break;
        }
    }
    if (KeyFrames.Num() == index)
    {
        PreKeyIndex = index - 1;
        NextKeyIndex = -1;
    }
    else
    {
        NextKeyIndex = index;
        PreKeyIndex = NextKeyIndex - 1;
    }
}

FTransform ISNKeyFrameInterface::CaclTargetAdditionalTransform(FTransform InitalTransform, float CurrentTime)
{
    int32 PreKeyIndex, NextKeyIndex;
    GetKeyIndexRangeOfTime(CurrentTime, PreKeyIndex, NextKeyIndex);

    const auto& KeyFrames = GetKeyFrames();
    if(PreKeyIndex == -1 && NextKeyIndex == -1) return InitalTransform;
    else if(PreKeyIndex != -1 && NextKeyIndex == -1) return KeyFrames[PreKeyIndex].Transform;
    else return KeyFrames[NextKeyIndex].Transform;
}

FTransform ISNKeyFrameInterface::GetTimeInterpTransform(FTransform InitalTransform, float LocalTime)
{
    int32 PreKeyIndex, NextKeyIndex;
    GetKeyIndexRangeOfTime(LocalTime, PreKeyIndex, NextKeyIndex);
    FTransform InterpTransform;
    FTransform PreTransform;
    FTransform NextTransform;
    float Alpha = 0.f;
    const auto& KeyFrames = GetKeyFrames();
    if(PreKeyIndex == -1 && NextKeyIndex == -1) return InitalTransform;
    else if(PreKeyIndex != -1 && NextKeyIndex == -1) return KeyFrames[PreKeyIndex].Transform;
    else if(PreKeyIndex == -1 && NextKeyIndex != -1)
    {
        PreTransform = InitalTransform;
        NextTransform = KeyFrames[NextKeyIndex].Transform;
        Alpha = LocalTime / KeyFrames[NextKeyIndex].LocalTime;
    }
    else
    {
        PreTransform = KeyFrames[PreKeyIndex].Transform;
        NextTransform = KeyFrames[NextKeyIndex].Transform;
        Alpha = (LocalTime - KeyFrames[PreKeyIndex].LocalTime) / (KeyFrames[NextKeyIndex].LocalTime - KeyFrames[PreKeyIndex].LocalTime);
    }
    InterpTransform.Blend(PreTransform, NextTransform, Alpha);
    return InterpTransform;
}
