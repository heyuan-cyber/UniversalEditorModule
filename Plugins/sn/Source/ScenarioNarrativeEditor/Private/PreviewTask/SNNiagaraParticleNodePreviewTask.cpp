#include "PreviewTask/SNNiagaraParticleNodePreviewTask.h"

#include "ScenarioNarrativeEditorDefines.h"
#include "NiagaraComponent.h"
#include "NiagaraComponentPool.h"
#include "Node/SNNiagaraParticleNode.h"
#include "Preview/SNPreviewContext.h"
#include "NiagaraFunctionLibrary.h"
#include "Utils.h"
#include "NiagaraSystemInstanceController.h"

#pragma optimize("", off)

#define LOCTEXT_NAMESPACE "FSNNiagaraParticleNodePreviewTask"

FSNNiagaraParticleNodePreviewTask::~FSNNiagaraParticleNodePreviewTask()
{
    for (auto AssetComp : AssetCompMapper)
    {
        auto Comp = AssetComp.Value;
        Comp->DestroyComponent();
    }

    AssetCompMapper.Empty();
}

void FSNNiagaraParticleNodePreviewTask::Enter(const FSNPreviewContext* Context)
{
    USNNiagaraParticleNode* Node = Cast<USNNiagaraParticleNode>(Context->Node);
    if (!Node->Particle)
        return;

    USkeletalMeshComponent* SkeletalMeshComponent = Context->Actor ? Context->Actor->GetComponentByClass<USkeletalMeshComponent>() : nullptr;

    // if (!AssetCompMapper.Contains(Context->Node))
    // {
    //     if (auto* NC = Context->Performer->SpawnNiagaraSystem(Node->Particle, SkeletalMeshComponent, FName(Node->SocketName),
    //                                                        Node->Offset.GetLocation(), Node->Offset.Rotator(), false))
    //     {
    //         NC->SetRandomSeedOffset(-Node->Particle->GetRandomSeed() + 10086);
    //         Node->Particle->SetCompileForEdit(true);
    //         SetupNiagaraComponent(NC);
    //         AssetCompMapper.Add(Context->Node, NC);
    //     }
    // }
}

void FSNNiagaraParticleNodePreviewTask::Sample(const FSNPreviewManager* Context, float CurrentTime, float PrevTime)
{
    USNNiagaraParticleNode* Node = Cast<USNNiagaraParticleNode>(Context->PreviewContext->Node);
    if (!Node->Particle || !AssetCompMapper.Contains(Context->PreviewContext->Node))
        return;

    UNiagaraComponent* NC = AssetCompMapper[Context->PreviewContext->Node];
    const float LocalTime = FMath::Max(0, CurrentTime - Node->BeginTime);

    if (NC->GetAsset() != Node->Particle)
    {
        NC->ResetSystem();
        NC->SetAsset(Node->Particle);
    }

    NC->SetRelativeTransform(Node->Offset);
    DrawDebugBox(Context->PreviewContext->Actor->GetWorld(), NC->GetComponentLocation(), FVector(5.f), FColor::Cyan, false, Utils::ONE_FRAME_TIME);

    if (FMath::IsNearlyEqual(LocalTime, NC->GetDesiredAge()))
        return;

    NC->SetPaused(false);
    NC->AdvanceSimulationByTime(LocalTime, Node->Particle->GetFixedTickDeltaTime());
    NC->SetDesiredAge(LocalTime);
    NC->SetPaused(true);
}

void FSNNiagaraParticleNodePreviewTask::Exit(const FSNPreviewContext* Context)
{
    USNNiagaraParticleNode* Node = Cast<USNNiagaraParticleNode>(Context->Node);
    if (!Node->Particle)
        return;

    if (!AssetCompMapper.Contains(Context->Node))
        return;

    UNiagaraComponent* NC = AssetCompMapper[Context->Node];
    NC->DeactivateImmediate();

    //Context->Player->DestroyNiagaraSystem(NC);
    AssetCompMapper.Remove(Context->Node);
}

void FSNNiagaraParticleNodePreviewTask::SetupNiagaraComponent(UNiagaraComponent* PreviewComponent)
{
    if (!PreviewComponent)
        return;

    // PreviewComponent->PoolingMethod = ENCPoolMethod::AutoRelease;
    PreviewComponent->CastShadow = 1;
    PreviewComponent->bCastDynamicShadow = 1;
    PreviewComponent->SetAllowScalability(true);
    PreviewComponent->SetForceSolo(true);
    PreviewComponent->SetAgeUpdateMode(ENiagaraAgeUpdateMode::DesiredAge);
    PreviewComponent->SetSeekDelta(Utils::ONE_FRAME_TIME / 3);
    PreviewComponent->SetLockDesiredAgeDeltaTimeToSeekDelta(false);
    PreviewComponent->SetRenderingEnabled(true);
    PreviewComponent->SetCanRenderWhileSeeking(false);
    PreviewComponent->Activate(true);
}

void FSNNiagaraParticleNodePreviewTask::Close(const FSNPreviewContext* Context)
{
    if (!Context || !Context->Node || !AssetCompMapper.Contains(Context->Node))
        return;

    //Context->Player->DestroyNiagaraSystem(AssetCompMapper[Context->Node]);
    AssetCompMapper.Remove(Context->Node);
}

#undef LOCTEXT_NAMESPACE
#pragma optimize("", on)
