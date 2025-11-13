#pragma once
#include "DataStyle/SNNodeStyle.h"

class FSNNiagaraParticleNodeStyle : public FSNNodeStyle
{
public:
    FSNNiagaraParticleNodeStyle();
    virtual ~FSNNiagaraParticleNodeStyle() override = default;

    virtual FString GetTypeName() const override;
};
