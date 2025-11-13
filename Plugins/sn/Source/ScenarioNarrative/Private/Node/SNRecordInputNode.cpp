#include "Node/SNRecordInputNode.h"

#include "Track/SNInputTrack.h"

TSet<UClass*> USNRecordInputNode::AttachedToTracks() const
{
    return { USNInputTrack::StaticClass() };
}

bool USNRecordInputNode::Validate() const
{
    return DisableTime >= 0 && DisableTime <= Length();
}

#if WITH_EDITOR
void USNRecordInputNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    DisableTime = FMath::Clamp(DisableTime, 0, Length());
}
#endif
