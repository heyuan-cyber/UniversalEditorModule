// RecordInputNode.h
// @author : xiangyp
// @date : 2025/06/05 17:47:47
// @desc : 记录输入节点

#pragma once
#include "SNNodeBase.h"
#include "SNRecordInputNode.generated.h"

UCLASS(MinimalAPI)
class USNRecordInputNode : public USNNodeBase
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DisableTime;

    virtual TSet<UClass*> AttachedToTracks() const override;

    virtual bool Validate() const override;

#if WITH_EDITOR
    virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

};
