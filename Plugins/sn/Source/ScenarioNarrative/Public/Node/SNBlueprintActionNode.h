// BlueprintActionNode.h
// @author : xiangyp
// @date : 2025/06/19 15:57:45
// @desc : 蓝图拓展Node基类

#pragma once
#include "SNNodeBase.h"
#include "SNBlueprintActionNode.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class SCENARIONARRATIVE_API USNBlueprintActionNode : public USNNodeBase
{
    GENERATED_BODY()
public:
    USNBlueprintActionNode() = default;
    virtual ~USNBlueprintActionNode() override = default;

    virtual TSet<UClass*> AttachedToTracks() const override;
    virtual bool Validate() const override;

    UFUNCTION(BlueprintNativeEvent)
    TSet<UClass*> AttachedTracks() const;

    UFUNCTION(BlueprintNativeEvent)
    bool CheckValid() const;

private:
    TSet<UClass*> AttachedTracks_Implementation() const;
    bool CheckValid_Implementation() const;

};
