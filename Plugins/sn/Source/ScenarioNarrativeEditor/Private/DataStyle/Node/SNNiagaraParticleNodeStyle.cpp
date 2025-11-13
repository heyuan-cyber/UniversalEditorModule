#include "SNNiagaraParticleNodeStyle.h"

FSNNiagaraParticleNodeStyle::FSNNiagaraParticleNodeStyle()
{
    BottomColor = FColor::FromHex("#01cafc");
}

FString FSNNiagaraParticleNodeStyle::GetTypeName() const
{
    return TEXT("Niagara粒子节点");
}
