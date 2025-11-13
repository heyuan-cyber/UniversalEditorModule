#include "Node/SNNiagaraParticleNode.h"

#include "Track/SNVfxTrack.h"
// #include "NiagaraSystem.h"

TSet<UClass*> USNNiagaraParticleNode::AttachedToTracks() const
{
    return { USNVfxTrack::StaticClass() };
}

bool USNNiagaraParticleNode::Validate() const
{
    return Particle != nullptr;
}
