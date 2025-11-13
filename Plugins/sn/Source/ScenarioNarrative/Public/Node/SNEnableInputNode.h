// EnableInputNode.h
// @author : xiangyp
// @date : 2025/06/05 16:45:55
// @desc : 输入控制节点

#pragma once
#include "SNNodeBase.h"
#include "SNEnableInputNode.generated.h"

UCLASS(MinimalAPI)
class USNEnableInputNode : public USNNodeBase
{
    GENERATED_BODY()
public:
    USNEnableInputNode();
    virtual ~USNEnableInputNode() override = default;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName="允许输入")
    bool bIsEnable = true;

    virtual TSet<UClass*> AttachedToTracks() const override;

#if WITH_EDITOR
    virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

};
