#include "SNVfxTrackStyle.h"

#include "ClassIconFinder.h"
#include "NiagaraSystem.h"

FSNVfxTrackStyle::FSNVfxTrackStyle()
{
    TrackIcon = FClassIconFinder::FindThumbnailForClass(UNiagaraSystem::StaticClass());
    TrackEdColor = FColor::FromHex("#01cafc");
}

FString FSNVfxTrackStyle::GetTypeName() const
{
    return TEXT("特效轨道");
}
