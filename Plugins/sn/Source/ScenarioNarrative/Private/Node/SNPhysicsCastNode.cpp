#include "Node/SNPhysicsCastNode.h"

#include "Track/SNPhysicsTrack.h"

USNPhysicsCastNode::USNPhysicsCastNode()
{
#if WITH_EDITOR
    bCanRename = false;
#endif
}

TSet<UClass*> USNPhysicsCastNode::AttachedToTracks() const
{
    return { USNPhysicsTrack::StaticClass() };
}

#if WITH_EDITOR
void USNPhysicsCastNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    if (PropertyChangedEvent.Property->NamePrivate != FName("Shape"))
        return;

    const UEnum* ShapePtr = StaticEnum<ESNTraceShapeType::Type>();
    DisplayName = FName(ShapePtr->GetNameStringByValue(Shape) + TEXT(" Cast"));
}
#endif
