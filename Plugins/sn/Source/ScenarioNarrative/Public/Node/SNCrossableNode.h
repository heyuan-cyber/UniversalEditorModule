#pragma once

#include "SNNodeBase.h"
#include "SNNodeBlend.h"
#include "SNCrossableNode.generated.h"

UCLASS(Abstract)
class SCENARIONARRATIVE_API USNCrossableNode : public USNNodeBase
{
    GENERATED_BODY()

public:
    USNCrossableNode() { BlendOut.bIsReverse = true; }

    UPROPERTY(EditInstanceOnly, Blueprintable, Category = "Blend", DisplayName = "In")
    FSNNodeBlend BlendIn;

    UPROPERTY(EditInstanceOnly, Blueprintable, Category = "Blend", DisplayName = "Out")
    FSNNodeBlend BlendOut;
};
