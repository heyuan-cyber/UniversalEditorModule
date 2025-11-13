#include "DataStyle/Track/SNCameraTrackStyle.h"

FSNCameraTrackStyle::FSNCameraTrackStyle()
{
    TrackEdColor = FLinearColor::FromSRGBColor(FColor(80, 123, 72));
    TrackIcon = FAppStyle::GetBrush("ClassIcon.CameraComponent");
}

FString FSNCameraTrackStyle::GetTypeName() const
{
    return TEXT("相机轨道");
}
