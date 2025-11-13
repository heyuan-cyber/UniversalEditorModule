#include "SNPerformerTrackStyle.h"
FSNPerformerTrackStyle::FSNPerformerTrackStyle()
{
    TrackEdColor = FLinearColor::FromSRGBColor(FColor(80, 123, 72));
    TrackIcon = FAppStyle::GetBrush("ClassIcon.AnimSequence");
}

FString FSNPerformerTrackStyle::GetTypeName() const
{
    return TEXT("演员轨道");
}
