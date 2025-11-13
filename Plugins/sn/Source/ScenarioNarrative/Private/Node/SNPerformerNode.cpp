#include "Node/SNPerformerNode.h"
#include "Track/SNPerformerTrack.h"

TSet<UClass*> USNPerformerNode::AttachedToTracks() const
{
    return
    {
        USNPerformerTrack::StaticClass()
    };
}

void USNPerformerNode::SetKeyFramesTime(int32 index, float NewTime)
{
    if (KeyFrames.Num() <= index)
    {
        return;
    }
    KeyFrames[index].LocalTime = NewTime;
}
