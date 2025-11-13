#include "Node/SNTranslationNode.h"

#include "Track/SNBehaviorTrack.h"

USNTranslationNode::USNTranslationNode()
{
#if WITH_EDITOR
    bCanRename = false;
#endif
}

TSet<UClass*> USNTranslationNode::AttachedToTracks() const
{
    return { USNBehaviorTrack::StaticClass() };
}

#if WITH_EDITOR
void USNTranslationNode::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
    DisplayName = FName(FString::Format(TEXT("位移 (方向{0}, 距离{1})"), { Direction.ToString(), Distance.ToString() }));
}
#endif