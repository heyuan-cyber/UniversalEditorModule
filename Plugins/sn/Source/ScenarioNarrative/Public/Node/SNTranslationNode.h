// TranslationNode.h
// @author : xiangyp
// @date : 2025/06/05 17:50:28
// @desc : 程序位移节点

#pragma once
#include "SNNodeBase.h"
#include "SNTranslationNode.generated.h"

UCLASS(MinimalAPI)
class USNTranslationNode : public USNNodeBase
{
    GENERATED_BODY()
public:
    USNTranslationNode();

    // UPROPERTY(EditAnyWhere, BlueprintReadWrite)
    // ERelativeTransformSpace Space;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Direction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Distance;

    virtual TSet<UClass*> AttachedToTracks() const override;

#if WITH_EDITOR
    virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
