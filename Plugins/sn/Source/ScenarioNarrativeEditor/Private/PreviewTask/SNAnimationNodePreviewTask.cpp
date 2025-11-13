#include "PreviewTask/SNAnimationNodePreviewTask.h"

#include "SNAssetBase.h"
#include "AnimPreviewInstance.h"
#include "ScenarioNarrativeEditorDefines.h"
#include "Utils.h"
#include "Animation/DebugSkelMeshComponent.h"
#include "Node/SNAnimationNode.h"
#include "Preview/SNPreviewActor.h"
#include "Preview/SNPreviewContext.h"

void FSNAnimationNodePreviewTask::Enter(const FSNPreviewContext* Context)
{
    if (!EnsureSkeleton(Context))
    {
        return;
    }

    USNAnimationNode* Node = Cast<USNAnimationNode>(Context->Node);

    ASNPreviewActor* PreviewActor = Cast<ASNPreviewActor>(Context->Actor);
    if (PreviewActor && Node->Animation)
    {
        if (UDebugSkelMeshComponent* SkelMeshComp = Cast<UDebugSkelMeshComponent>(
            PreviewActor->GetComponentByClass(UDebugSkelMeshComponent::StaticClass())))
        {
            SkelMeshComp->EnablePreview(true, Node->Animation);
            SkelMeshComp->PreviewInstance->SetPlaying(false);
            SkelMeshComp->PreviewInstance->SetLooping(false);
            SkelMeshComp->PreviewInstance->SetPosition(Node->Animation->GetTimeAtFrame(Node->BeginFrame));

            AttachedComponents.Add(Context, Utils::CreateAttachedObjectToSkeleton(SkelMeshComp, Node->Animation->GetSkeleton()));
        }
    }
}

void FSNAnimationNodePreviewTask::Sample(const FSNPreviewManager* Context, float CurrentTime, float PrevTime)
{
    if (!EnsureSkeleton(Context->PreviewContext.Get()))
    {
        return;
    }

    USNAnimationNode* Node = Cast<USNAnimationNode>(Context->PreviewContext->Node);

    ASNPreviewActor* PreviewActor = Cast<ASNPreviewActor>(Context->PreviewContext->Actor);
    if (PreviewActor && Node->Animation)
    {
        if (UDebugSkelMeshComponent* SkelMeshComp = Cast<UDebugSkelMeshComponent>(
            PreviewActor->GetComponentByClass(UDebugSkelMeshComponent::StaticClass())))
        {
            if (SkelMeshComp->PreviewInstance->GetAnimSequence() != Node->Animation)
                SkelMeshComp->EnablePreview(true, Node->Animation);

            const float BeginTime = Node->Animation->GetTimeAtFrame(Node->BeginFrame);
            const float LocalTime = (CurrentTime - Node->BeginTime) * Node->PlayRate + BeginTime;
            const float PrevLocalTime = (PrevTime - Node->BeginTime) * Node->PlayRate + BeginTime;
            SkelMeshComp->PreviewInstance->SetPositionWithPreviousTime(LocalTime, PrevLocalTime);
        }
    }
}

void FSNAnimationNodePreviewTask::Exit(const FSNPreviewContext* Context)
{
    if (!EnsureSkeleton(Context))
    {
        return;
    }

    USNAnimationNode* Node = Cast<USNAnimationNode>(Context->Node);
    ASNPreviewActor* PreviewActor = Cast<ASNPreviewActor>(Context->Actor);
    if (PreviewActor && Node->Animation)
    {
        if (UDebugSkelMeshComponent* SkelMeshComp = Cast<UDebugSkelMeshComponent>(
            PreviewActor->GetComponentByClass(UDebugSkelMeshComponent::StaticClass())))
        {
            const float EndTime = Node->EndFrame <= 0 || Node->EndFrame > Node->Animation->GetNumberOfSampledKeys()
                                      ? Node->Animation->GetPlayLength()
                                      : Node->Animation->GetTimeAtFrame(Node->EndFrame);
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

bool FSNAnimationNodePreviewTask::EnsureSkeleton(const FSNPreviewContext* Context) const
{
    if (!Context->Actor)
    {
        return false;
    }

    USNAnimationNode* Node = Cast<USNAnimationNode>(Context->Node);

    ASNPreviewActor* PreviewActor = Cast<ASNPreviewActor>(Context->Actor);
    if (PreviewActor && Node->Animation)
    {
        if (UDebugSkelMeshComponent* SkelMeshComp = Cast<UDebugSkelMeshComponent>(
            PreviewActor->GetComponentByClass(UDebugSkelMeshComponent::StaticClass())))
        {
            auto Skeleton = Node->Animation->GetSkeleton();
            auto Mesh = Skeleton ? Skeleton->GetAssetPreviewMesh(Node->Animation) : nullptr;

            if (Skeleton && Mesh)
            {
                SkelMeshComp->SetSkeletalMesh(Mesh);
            }
            return true;
        }
    }

    return false;
}
