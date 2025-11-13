#include "Node/SNEnableInputNode.h"

#include "Track/SNInputTrack.h"

USNEnableInputNode::USNEnableInputNode()
{
#if WITH_EDITOR
    bCanRename = false;
#endif

}

TSet<UClass*> USNEnableInputNode::AttachedToTracks() const
{
    return { USNInputTrack::StaticClass() };
}

#if WITH_EDITOR
void USNEnableInputNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    DisplayName = FName(bIsEnable ? TEXT("允许输入") : TEXT("禁止输入"));
}
#endif
