#include "SNMontageSetTrackStyle.h"
FSNMontageSetTrackStyle::FSNMontageSetTrackStyle()
{
    TrackEdColor = FLinearColor::FromSRGBColor(FColor(80, 123, 72));
    TrackIcon = FAppStyle::GetBrush("ClassIcon.DialogueWave");
}

FString FSNMontageSetTrackStyle::GetTypeName() const
{
    return TEXT("蒙太奇轨道");
}
