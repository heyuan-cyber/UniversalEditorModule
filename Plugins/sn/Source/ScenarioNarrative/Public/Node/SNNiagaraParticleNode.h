// NiagaraEffectNode.h
// @author : xiangyp
// @date : 2025/05/20 15:52:43
// @desc : 播Niagara粒子系统的节点

#pragma once
#include "SNNodeBase.h"
#include "SNNiagaraParticleNode.generated.h"

class USNNiagaraComponent;
class UNiagaraSystem;

UCLASS(Blueprintable)
class SCENARIONARRATIVE_API USNNiagaraParticleNode : public USNNodeBase
{
    GENERATED_BODY()
public:
    USNNiagaraParticleNode() = default;
    virtual ~USNNiagaraParticleNode() override = default;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UNiagaraSystem> Particle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SocketName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FTransform Offset;

    virtual TSet<UClass*> AttachedToTracks() const override;

    virtual bool Validate() const override;
};
