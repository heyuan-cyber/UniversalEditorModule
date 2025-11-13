#include "Node/SNBlueprintActionNode.h"

#include "Track/SNTrackBase.h"

TSet<UClass*> USNBlueprintActionNode::AttachedToTracks() const
{
    return AttachedTracks();
}

bool USNBlueprintActionNode::Validate() const
{
    return CheckValid();
}

TSet<UClass*> USNBlueprintActionNode::AttachedTracks_Implementation() const
{
    return { USNTrackBase::StaticClass() };
}

bool USNBlueprintActionNode::CheckValid_Implementation() const
{
    return IF_WITH_EDITOR(bIsValid, true);
}
