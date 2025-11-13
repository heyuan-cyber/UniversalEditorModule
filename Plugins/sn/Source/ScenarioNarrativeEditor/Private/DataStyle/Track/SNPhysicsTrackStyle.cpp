#include "SNPhysicsTrackStyle.h"

#include "SNTypeRegistryHelper.h"

FSNPhysicsTrackStyle::FSNPhysicsTrackStyle()
{
    TrackEdColor = FColor::FromHex("#ff8600");
    TrackIcon = FSNTypeRegistryHelper::Get().GetBrush("Icons.PhysicsTrack");
}

FString FSNPhysicsTrackStyle::GetTypeName() const
{
    return TEXT("物理轨道");
}
