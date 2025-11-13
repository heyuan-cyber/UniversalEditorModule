#include "SNDialogTrackStyle.h"

FSNDialogTrackStyle::FSNDialogTrackStyle()
{
    TrackEdColor = FLinearColor::FromSRGBColor(FColor(80, 123, 72));
    TrackIcon = FAppStyle::GetBrush("ClassIcon.DialogueWave");
}

FString FSNDialogTrackStyle::GetTypeName() const
{
    return TEXT("对话轨道");
}
