#include "Node/SNMontageSetNode.h"

#include "ScenarioNarrativeDefines.h"
#include "MontageSet/SNMontageSetAsset.h"

#include "Track/SNMontageSetTrack.h"

USNMontageSetNode::USNMontageSetNode()
{
#if WITH_EDITOR
    bResizable = false;
#endif
}

TSet<UClass*> USNMontageSetNode::AttachedToTracks() const
{
    return
    {
        USNMontageSetTrack::StaticClass()
    };
}

bool USNMontageSetNode::Validate() const
{
    return Super::Validate() && MontageSet &&
        MontageSet->Clips.ContainsByPredicate([&](const TObjectPtr<USNAnimationClipData>& Data) { return Data->Name == ClipName; });
}

TArray<FString> USNMontageSetNode::GetAllClipNames() const
{
    TArray<FString> Options;

    if (!MontageSet)
    {
        return Options;
    }

    for (const auto& Clip : MontageSet->Clips)
    {
        Options.Add(Clip->Name);
    }

    return Options;
}

#if WITH_EDITOR
void USNMontageSetNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    DisplayName = Validate() ? FName(FString::Printf(TEXT("%s[%s]"), *MontageSet->GetName(), *ClipName)) : "Invalid MontageSet";

    if (!Validate())
    {
        return;
    }

    BeginTime = FMath::Max(0, BeginTime);
    const auto* ClipData = MontageSet->GetClip(ClipName);

    const float BeginFrameTime = MontageSet->Animation->GetTimeAtFrame(ClipData->BeginFrame);
    const int32 TargetEndFrame = TargetFrame > 0 ? ClipData->BeginFrame + TargetFrame : ClipData->EndFrame;
    const float EndFrameTime = SN_ONE_FRAME_TIME * TargetEndFrame; // MontageSet->Animation->GetTimeAtFrame(TargetEndFrame);

    EndTime = FMath::Max(BeginTime + GetMinLength(), BeginTime + (EndFrameTime - BeginFrameTime) / ClipData->PlayRate);
}
#endif
