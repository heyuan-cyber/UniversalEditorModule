#include "Node/SNAnimationNode.h"

#include "Track/SNAnimationTrack.h"
#include "ScenarioNarrativeDefines.h"
#include "ScenarioNarrativeFunctionLibrary.h"

USNAnimationNode::USNAnimationNode()
{
#if WITH_EDITOR
    bResizable = false;
    bCanRename = false;
#endif
}

float USNAnimationNode::GetMaxLength() const
{
    if (!Animation)
    {
        return GetMinLength();
    }

    float RateClamped = PlayRate == 0 ? 1.f : FMath::Abs(PlayRate);

    if (TargetFrame > 0)
    {
        return FScenarioNarrativeFunctionLibrary::GetAnimationTimeAtFrame(Animation, TargetFrame) / RateClamped;
    }

    float EndTimeWithRate = (EndFrame <= 0 ? Animation->GetPlayLength() : FScenarioNarrativeFunctionLibrary::GetAnimationTimeAtFrame(Animation, EndFrame)) / RateClamped;
    float BeginTimeWithRate = FScenarioNarrativeFunctionLibrary::GetAnimationTimeAtFrame(Animation, BeginFrame) / RateClamped;

    return EndTimeWithRate - BeginTimeWithRate;
}

TSet<UClass*> USNAnimationNode::AttachedToTracks() const
{
    return
    {
        USNAnimationTrack::StaticClass()
    };
}

bool USNAnimationNode::Validate() const
{
    return Super::Validate() && Animation && PlayRate != 0;
}

#if WITH_EDITOR
void USNAnimationNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    PlayRate = FMath::Max(0, PlayRate);
    DisplayName = Validate() ? Animation->GetFName() : "Empty Animation";

    if (!Validate())
    {
        return;
    }

    BeginFrame = FMath::Max(BeginFrame, 0);
    BeginTime = FMath::Max(0, BeginTime);

    EndFrame = FMath::Clamp(EndFrame, 0, Animation->GetNumberOfSampledKeys() - 1);
    if (TargetFrame > 0)
    {
        float TargetTime = SN_ONE_FRAME_TIME * TargetFrame;
        PlayRate = FMath::Max(0, (Animation->GetTimeAtFrame(EndFrame) - Animation->GetTimeAtFrame(BeginFrame)) / TargetTime);
    }

    if (EndFrame > 0)
    {
        EndTime = BeginTime + (Animation->GetTimeAtFrame(EndFrame) - Animation->GetTimeAtFrame(BeginFrame)) / PlayRate;
    }
    else
    {
        EndTime = BeginTime + Animation->GetPlayLength() / PlayRate;
    }
}
#endif
