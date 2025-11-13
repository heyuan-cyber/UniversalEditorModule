#include "PreviewTask/SNMontageSetNodePreviewTask.h"

#include "SNAssetBase.h"
#include "AnimPreviewInstance.h"
#include "ScenarioNarrativeEditorDefines.h"
#include "Utils.h"
#include "Animation/DebugSkelMeshComponent.h"
#include "MontageSet/SNMontageSetAsset.h"
#include "Node/SNMontageSetNode.h"
#include "Preview/SNPreviewActor.h"
#include "Preview/SNPreviewContext.h"

void FSNMontageSetNodePreviewTask::Enter(const FSNPreviewContext* Context)
{
    if (!EnsureSkeleton(Context))
    {
        return;
    }

    USNMontageSetNode* Node = Cast<USNMontageSetNode>(Context->Node);

    ASNPreviewActor* PreviewActor = Cast<ASNPreviewActor>(Context->Actor);
    if (PreviewActor && Node->Validate())
    {
        if (UDebugSkelMeshComponent* SkelMeshComp = Cast<UDebugSkelMeshComponent>(
            PreviewActor->GetComponentByClass(UDebugSkelMeshComponent::StaticClass())))
        {
            SkelMeshComp->EnablePreview(true, Node->MontageSet->Animation);
            SkelMeshComp->PreviewInstance->SetPlaying(false);
            SkelMeshComp->PreviewInstance->SetLooping(false);
            SkelMeshComp->PreviewInstance->SetPosition(
                Node->MontageSet->Animation->GetTimeAtFrame(Node->MontageSet->GetClip(Node->ClipName)->BeginFrame));

            AttachedComponents.Add(Context, Utils::CreateAttachedObjectToSkeleton(SkelMeshComp, Node->MontageSet->Animation->GetSkeleton()));
        }
    }
}

void FSNMontageSetNodePreviewTask::Sample(const FSNPreviewManager* Context, float CurrentTime, float PrevTime)
{
    if (!EnsureSkeleton(Context->PreviewContext.Get()))
    {
        return;
    }

    USNMontageSetNode* Node = Cast<USNMontageSetNode>(Context->PreviewContext->Node);

    ASNPreviewActor* PreviewActor = Cast<ASNPreviewActor>(Context->PreviewContext->Actor);
    if (PreviewActor && Node->Validate())
    {
        const auto* ClipData = Node->MontageSet->GetClip(Node->ClipName);
        if (UDebugSkelMeshComponent* SkelMeshComp = Cast<UDebugSkelMeshComponent>(
            PreviewActor->GetComponentByClass(UDebugSkelMeshComponent::StaticClass())))
        {
            const float BeginTime = Node->MontageSet->Animation->GetTimeAtFrame(ClipData->BeginFrame);
            const float LocalTime = CurrentTime - Node->BeginTime + BeginTime;
            const float PrevLocalTime = PrevTime - Node->BeginTime + BeginTime;
            SkelMeshComp->PreviewInstance->SetPositionWithPreviousTime(LocalTime * ClipData->PlayRate, PrevLocalTime * ClipData->PlayRate);
        }
    }
}

void FSNMontageSetNodePreviewTask::Exit(const FSNPreviewContext* Context)
{
    if (!EnsureSkeleton(Context))
    {
        return;
    }

    USNMontageSetNode* Node = Cast<USNMontageSetNode>(Context->Node);
    ASNPreviewActor* PreviewActor = Cast<ASNPreviewActor>(Context->Actor);
    if (PreviewActor && Node->Validate())
    {
        if (UDebugSkelMeshComponent* SkelMeshComp = Cast<UDebugSkelMeshComponent>(
            PreviewActor->GetComponentByClass(UDebugSkelMeshComponent::StaticClass())))
        {
            const auto* ClipData = Node->MontageSet->GetClip(Node->ClipName);
            const float EndTime = ClipData->EndFrame <= 0 || ClipData->EndFrame > Node->MontageSet->Animation->GetNumberOfSampledKeys()
                                      ? Node->MontageSet->Animation->GetPlayLength()
                                      : Node->MontageSet->Animation->GetTimeAtFrame(ClipData->EndFrame);
            SkelMeshComp->SetPosition(EndTime);
        }
    }

    if (AttachedComponents.Contains(Context))
    {
        for (const auto& Comp : AttachedComponents[Context])
        {
            Comp->DestroyComponent();
        }
        AttachedComponents.Remove(Context);
    }
}

bool FSNMontageSetNodePreviewTask::EnsureSkeleton(const FSNPreviewContext* Context) const
{
    if (!Context->Actor)
    {
        return false;
    }

    USNMontageSetNode* Node = Cast<USNMontageSetNode>(Context->Node);

    ASNPreviewActor* PreviewActor = Cast<ASNPreviewActor>(Context->Actor);
    if (PreviewActor && Node->Validate())
    {
        if (UDebugSkelMeshComponent* SkelMeshComp = Cast<UDebugSkelMeshComponent>(
            PreviewActor->GetComponentByClass(UDebugSkelMeshComponent::StaticClass())))
        {
            auto Skeleton = Node->MontageSet->Animation->GetSkeleton();
            auto Mesh = Skeleton ? Skeleton->GetAssetPreviewMesh(Node->MontageSet->Animation) : nullptr;

            if (Skeleton && Mesh)
            {
                SkelMeshComp->SetSkeletalMesh(Mesh);
            }
            return true;
        }
    }

    return false;
}
