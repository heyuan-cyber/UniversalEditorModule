#include "SNInputTrackStyle.h"

#include "SNTypeRegistryHelper.h"

FSNInputTrackStyle::FSNInputTrackStyle()
{
    TrackEdColor = FColor(127, 255, 255);
    TrackIcon = FSNTypeRegistryHelper::Get().GetBrush("Icons.InputTrack");
}

FString FSNInputTrackStyle::GetTypeName() const
{
    return TEXT("输入轨道");
}

float FSNInputTrackStyle::GetDesiredHeight() const
{
    return 30.f;
}
