#include "SNBehaviorTrackStyle.h"

#include "SNTypeRegistryHelper.h"

FSNBehaviorTrackStyle::FSNBehaviorTrackStyle()
{
    TrackEdColor = FColor(149, 70, 255);
    TrackIcon = FSNTypeRegistryHelper::Get().GetBrush("Icons.ActionTrack");
}

FString FSNBehaviorTrackStyle::GetTypeName() const
{
    return TEXT("行为轨道");
}
