#include "DataStyle/Track/SNAnimationTrackStyle.h"

FSNAnimationTrackStyle::FSNAnimationTrackStyle()
{
    TrackEdColor = FLinearColor::FromSRGBColor(FColor(80, 123, 72));
    TrackIcon = FAppStyle::GetBrush("ClassIcon.AnimSequence");
}

FString FSNAnimationTrackStyle::GetTypeName() const
{
    return TEXT("动画轨道");
}
