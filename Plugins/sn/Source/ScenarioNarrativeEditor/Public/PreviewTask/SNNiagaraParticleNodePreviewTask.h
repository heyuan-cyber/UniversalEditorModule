#pragma once
#include "Task/SNPreviewTaskBase.h"

class UNiagaraComponent;
class USNAssetBase;

class FSNNiagaraParticleNodePreviewTask : public FSNPreviewTaskBase
{
public:
    FSNNiagaraParticleNodePreviewTask() = default;
    virtual ~FSNNiagaraParticleNodePreviewTask() override;

    virtual void Enter(const FSNPreviewContext* Context) override;
    virtual void Sample(const FSNPreviewManager* Context, float CurrentTime, float PrevTime) override;
    virtual void Exit(const FSNPreviewContext* Context) override;

    virtual void Close(const FSNPreviewContext* Context) override;

private:
    TMap<const USNAssetBase*, UNiagaraComponent*> AssetCompMapper;

    void SetupNiagaraComponent(UNiagaraComponent* PreviewComponent);
};
