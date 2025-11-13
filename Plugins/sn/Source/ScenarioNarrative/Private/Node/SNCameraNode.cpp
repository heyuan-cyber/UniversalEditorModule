#include "Node/SNCameraNode.h"

#include "Track/SNCameraTrack.H"
#include "ScenarioNarrativeDefines.h"
#include "ScenarioNarrativeFunctionLibrary.h"
#include "ScenarioNarrativeEditor/Public/SNSequencerContext.h"
#include "ScenarioNarrativeEditor/Public/SequencerEditor/SNNodeWidget.h"

USNCameraNode::USNCameraNode()
{
#if WITH_EDITOR
    bCanRename = false;
#endif

}

TSet<UClass*> USNCameraNode::AttachedToTracks() const
{
    return { USNCameraTrack::StaticClass() };
}

#if WITH_EDITOR
void USNCameraNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    const FName PropertyName = PropertyChangedEvent.GetPropertyName();

    if (PropertyName == GET_MEMBER_NAME_CHECKED(USNCameraNode, KeyFrames))
    {
        for(FSNCameraKeyFrame SNKeyFrame : KeyFrames)
        {

        }
    }
}
#endif

void USNCameraNode::SetKeyFramesTime(int32 index, float NewTime)
{
    if (KeyFrames.Num() <= index)
    {
        return;
    }
    KeyFrames[index].Time = NewTime;
}

void USNCameraNode::SortKetFramesByTime()
{
    // 升序排序
    KeyFrames.Sort([](const FSNCameraKeyFrame& A, const FSNCameraKeyFrame& B) {
        return A.Time < B.Time;
    });
}

TArray<FSNCameraKeyFrame> USNCameraNode::GetKetFramesByCurrentTime(float CurrentTime)
{
    SortKetFramesByTime();
    TArray<FSNCameraKeyFrame> KeyFrameArray;
    KeyFrameArray.AddDefaulted(2);

    int32 Low = 0;
    int32 High = KeyFrames.Num() - 1;
    int32 LastLessEqualIndex = -1;

    while (Low <= High)
    {
        int32 Mid = (Low + High) / 2;
        const FSNCameraKeyFrame& MidFrame = KeyFrames[Mid];

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
        KeyFrameArray[0] = KeyFrames[LastLessEqualIndex];

        if (LastLessEqualIndex + 1 < KeyFrames.Num())
        {
            KeyFrameArray[1] = KeyFrames[LastLessEqualIndex + 1];
        }
    }

    return KeyFrameArray;
}

TSharedPtr<FJsonObject> USNCameraNode::NodeDataToJson()
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

    JsonObject->SetNumberField("StartTime", BeginTime);
    JsonObject->SetNumberField("EndTime", EndTime);

    // 添加关键帧数组
    TArray<TSharedPtr<FJsonValue>> KeyframesArray;
    for (const FSNCameraKeyFrame Keyframe : KeyFrames)
    {
        TSharedPtr<FJsonObject> KeyFrameJsonObject = MakeShareable(new FJsonObject);

        KeyFrameJsonObject->SetNumberField("AbsoluteTime", Keyframe.AbsoluteTime);
        KeyFrameJsonObject->SetNumberField("Time", Keyframe.Time);

        TSharedPtr<FJsonObject> KeyFramePositionJsonObject = MakeShareable(new FJsonObject);
        KeyFramePositionJsonObject->SetNumberField("x", Keyframe.Position.X);
        KeyFramePositionJsonObject->SetNumberField("y", Keyframe.Position.Y);
        KeyFramePositionJsonObject->SetNumberField("z", Keyframe.Position.Z);

        TSharedPtr<FJsonObject> KeyFrameRotationJsonObject = MakeShareable(new FJsonObject);
        KeyFrameRotationJsonObject->SetNumberField("Pitch", Keyframe.Rotation.Pitch);
        KeyFrameRotationJsonObject->SetNumberField("Roll", Keyframe.Rotation.Roll);
        KeyFrameRotationJsonObject->SetNumberField("Yaw", Keyframe.Rotation.Yaw);

        KeyFrameJsonObject->SetObjectField("Position", KeyFramePositionJsonObject);
        KeyFrameJsonObject->SetObjectField("Rotation", KeyFrameRotationJsonObject);

        TSharedPtr<FJsonValue> KeyframeValue = MakeShareable(new FJsonValueObject(KeyFrameJsonObject));
        KeyframesArray.Add(KeyframeValue);
    }
    JsonObject->SetArrayField("Keyframes", KeyframesArray);

    return JsonObject;
}
