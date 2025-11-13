#include "SNSceneTrackStyle.h"

FSNSceneTrackStyle::FSNSceneTrackStyle()
{
    TrackEdColor = FLinearColor::FromSRGBColor(FColor(80, 123, 72));
    TrackIcon = FAppStyle::GetBrush("ClassIcon.LandscapeComponent");
}

FString FSNSceneTrackStyle::GetTypeName() const
{
    return TEXT("场景轨道");
}
