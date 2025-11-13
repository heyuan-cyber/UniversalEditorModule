#include "Node/SNSplineCameraNode.h"

#include "Components/SplineComponent.h"
#include "Track/SNCameraTrack.H"

USNSplineCameraNode::USNSplineCameraNode()
{
#if WITH_EDITOR
    bCanRename = false;
#endif

}

TSet<UClass*> USNSplineCameraNode::AttachedToTracks() const
{
    return { USNCameraTrack::StaticClass() };
}

#if WITH_EDITOR
void USNSplineCameraNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{

}
#endif

void USNSplineCameraNode::SetKeyFramesTime(int32 index, float NewTime)
{
    if (KeyFrames.Num() <= index)
    {
        return;
    }
    KeyFrames[index].Time = NewTime;
}

bool USNSplineCameraNode::GetCameraTransformAtTime(float CurrentTime, FVector& OutLocation, FRotator& OutRotation) const
{
    if (!SplineComponent.IsValid() || KeyFrames.Num() == 0)
    {
        return false;
    }

    // 确保关键帧按时间排序
    const_cast<USNSplineCameraNode*>(this)->SortKeyFramesByTime();

    // 查找当前时间所在的关键帧区间
    int32 PrevKeyFrame = -1;
    int32 NextKeyFrame = -1;
    float Alpha = 0.0f;

    float SplineDistance = GetInterpolatedSplineDistance(CurrentTime, PrevKeyFrame, NextKeyFrame, Alpha);

    // 如果找不到有效关键帧
    if (PrevKeyFrame == -1 && NextKeyFrame == -1)
    {
        return false;
    }

    // 只有一个关键帧或当前时间在第一个关键帧之前
    if (NextKeyFrame == -1)
    {
        int32 UseKeyFrame = (PrevKeyFrame != -1) ? PrevKeyFrame : 0;
        SplineDistance = GetSplineDistanceForKeyFrame(UseKeyFrame);

        // 获取样条线上的位置和旋转
        OutLocation = SplineComponent->GetLocationAtDistanceAlongSpline(SplineDistance, ESplineCoordinateSpace::World);
        OutRotation = SplineComponent->GetRotationAtDistanceAlongSpline(SplineDistance, ESplineCoordinateSpace::World);

        // 应用关键帧的旋转偏移
        if (PrevKeyFrame != -1)
        {
            OutRotation = KeyFrames[PrevKeyFrame].Rotation;
        }
        return true;
    }

    // 当前时间在最后一个关键帧之后
    if (PrevKeyFrame == -1)
    {
        SplineDistance = GetSplineDistanceForKeyFrame(NextKeyFrame);

        // 获取样条线上的位置和旋转
        OutLocation = SplineComponent->GetLocationAtDistanceAlongSpline(SplineDistance, ESplineCoordinateSpace::World);
        OutRotation = SplineComponent->GetRotationAtDistanceAlongSpline(SplineDistance, ESplineCoordinateSpace::World);

        // 应用关键帧的旋转偏移
        OutRotation = KeyFrames[NextKeyFrame].Rotation;
        return true;
    }

    // 在两个关键帧之间进行插值
    FSNSplineCameraKeyFrame& PrevFrame = KeyFrames[PrevKeyFrame];
    FSNSplineCameraKeyFrame& NextFrame = KeyFrames[NextKeyFrame];

    float PrevDistance = GetSplineDistanceForKeyFrame(PrevKeyFrame);
    float NextDistance = GetSplineDistanceForKeyFrame(NextKeyFrame);

    // 计算插值后的样条线距离
    SplineDistance = FMath::Lerp(PrevDistance, NextDistance, Alpha);

    // 获取样条线上的位置
    OutLocation = SplineComponent->GetLocationAtDistanceAlongSpline(SplineDistance, ESplineCoordinateSpace::World);

    // 插值旋转
    OutRotation = FMath::Lerp(PrevFrame.Rotation, NextFrame.Rotation, Alpha);

    return true;
}

float USNSplineCameraNode::GetSplineDistanceForKeyFrame(int32 KeyFrameIndex) const
{
    if (!SplineComponent.IsValid() || !KeyFrames.IsValidIndex(KeyFrameIndex))
    {
        return 0.0f;
    }

    const FSNSplineCameraKeyFrame& KeyFrame = KeyFrames[KeyFrameIndex];

    // 如果关键帧有指定的样条点索引，使用该点
    if (KeyFrame.PointInputKey >= 0 && KeyFrame.PointInputKey < SplineComponent->GetNumberOfSplinePoints())
    {
        return SplineComponent->GetDistanceAlongSplineAtSplinePoint(KeyFrame.PointInputKey);
    }

    // 否则，根据时间比例计算距离
    float MaxTime = 0.0f;
    for (const FSNSplineCameraKeyFrame& Frame : KeyFrames)
    {
        if (Frame.Time > MaxTime)
        {
            MaxTime = Frame.Time;
        }
    }

    if (MaxTime > 0.0f)
    {
        float TimeRatio = KeyFrame.Time / MaxTime;
        return TimeRatio * SplineComponent->GetSplineLength();
    }

    return 0.0f;
}

float USNSplineCameraNode::GetInterpolatedSplineDistance(float CurrentTime, int32& OutPrevKeyFrame, int32& OutNextKeyFrame, float& OutAlpha) const
{
    OutPrevKeyFrame = -1;
    OutNextKeyFrame = -1;
    OutAlpha = 0.0f;

    if (KeyFrames.Num() == 0)
    {
        return 0.0f;
    }

    // 查找当前时间所在的关键帧区间
    for (int32 i = 0; i < KeyFrames.Num(); i++)
    {
        if (KeyFrames[i].Time <= CurrentTime)
        {
            OutPrevKeyFrame = i;
        }
        else
        {
            if (i > 0)
            {
                OutNextKeyFrame = i;
                break;
            }
            else
            {
                OutNextKeyFrame = i;
                break;
            }
        }
    }

    if (OutPrevKeyFrame == -1 && OutNextKeyFrame != -1)
    {
        return GetSplineDistanceForKeyFrame(OutNextKeyFrame);
    }
    else if (OutPrevKeyFrame != -1 && OutNextKeyFrame == -1)
    {
        return GetSplineDistanceForKeyFrame(OutPrevKeyFrame);
    }
    else if (OutPrevKeyFrame != -1 && OutNextKeyFrame != -1)
    {
        FSNSplineCameraKeyFrame& PrevFrame = KeyFrames[OutPrevKeyFrame];
        FSNSplineCameraKeyFrame& NextFrame = KeyFrames[OutNextKeyFrame];

        float TimeRange = NextFrame.Time - PrevFrame.Time;
        if (TimeRange > 0.0f)
        {
            OutAlpha = (CurrentTime - PrevFrame.Time) / TimeRange;
        }

        // 计算插值后的距离
        float PrevDistance = GetSplineDistanceForKeyFrame(OutPrevKeyFrame);
        float NextDistance = GetSplineDistanceForKeyFrame(OutNextKeyFrame);

        return FMath::Lerp(PrevDistance, NextDistance, OutAlpha);
    }

    return 0.0f;
}

void USNSplineCameraNode::SortKeyFramesByTime()
{
    KeyFrames.Sort([](const FSNSplineCameraKeyFrame& A, const FSNSplineCameraKeyFrame& B) {
        return A.Time < B.Time;
    });
}

TArray<FSNSplineCameraKeyFrame> USNSplineCameraNode::GetKeyFramesByCurrentTime(float CurrentTime)
{
    SortKeyFramesByTime();
    TArray<FSNSplineCameraKeyFrame> KeyFrameArray;
    KeyFrameArray.AddDefaulted(2);

    int32 Low = 0;
    int32 High = KeyFrames.Num() - 1;
    int32 LastLessEqualIndex = -1;

    while (Low <= High)
    {
        int32 Mid = (Low + High) / 2;
        const FSNSplineCameraKeyFrame& MidFrame = KeyFrames[Mid];

        if (MidFrame.Time <= CurrentTime)
        {
            LastLessEqualIndex = Mid;
            Low = Mid + 1;
        }
        else
        {
            High = Mid - 1;
        }
    }

    if (LastLessEqualIndex >= 0)
    {
        KeyFrames[LastLessEqualIndex].PointInputKey = LastLessEqualIndex;
        KeyFrameArray[0] = KeyFrames[LastLessEqualIndex];

        if (LastLessEqualIndex + 1 < KeyFrames.Num())
        {
            KeyFrames[LastLessEqualIndex + 1].PointInputKey = LastLessEqualIndex + 1;
            KeyFrameArray[1] = KeyFrames[LastLessEqualIndex + 1];
        }
    }

    return KeyFrameArray;
}

TSharedPtr<FJsonObject> USNSplineCameraNode::NodeDataToJson()
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

    JsonObject->SetNumberField("StartTime", BeginTime);
    JsonObject->SetNumberField("EndTime", EndTime);

    // 添加关键帧数组
    TArray<TSharedPtr<FJsonValue>> KeyframesArray;
    for (const FSNSplineCameraKeyFrame Keyframe : KeyFrames)
    {
        TSharedPtr<FJsonObject> KeyFrameJsonObject = MakeShareable(new FJsonObject);

        KeyFrameJsonObject->SetNumberField("Time", Keyframe.Time);

        TSharedPtr<FJsonValue> KeyframeValue = MakeShareable(new FJsonValueObject(KeyFrameJsonObject));
        KeyframesArray.Add(KeyframeValue);
    }
    JsonObject->SetArrayField("Keyframes", KeyframesArray);

    return JsonObject;
}
