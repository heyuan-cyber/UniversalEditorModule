#pragma once

#include "SNCrossableNode.h"
#include "SNAnimationNode.generated.h"

UCLASS()
class SCENARIONARRATIVE_API USNAnimationNode : public USNCrossableNode
{
    GENERATED_BODY()

public:
    USNAnimationNode();
    virtual ~USNAnimationNode() override = default;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAnimSequenceBase> Animation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PlayRate = 1.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 BeginFrame = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ToolTip = "<=0 表示整个动画"))
    int32 EndFrame = -1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "<=0 表示该特性不启用"))
    int32 TargetFrame = -1;

    virtual float GetMaxLength() const override;
    virtual TSet<UClass*> AttachedToTracks() const override;
    virtual bool Validate() const override;

#if WITH_EDITOR
    virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
