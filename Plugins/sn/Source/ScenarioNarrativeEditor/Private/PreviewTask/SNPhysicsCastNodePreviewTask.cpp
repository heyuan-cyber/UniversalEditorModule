#include "PreviewTask/SNPhysicsCastNodePreviewTask.h"

#include "ScenarioNarrativeEditorDefines.h"
#include "KismetTraceUtils.h"
#include "Node/SNPhysicsCastNode.h"
#include "Preview/SNPreviewContext.h"
#include "DrawDebugHelpers.h"
#include "Utils.h"

FSNPhysicsCastNodePreviewTask::FSNPhysicsCastNodePreviewTask()
    : Shape(MakeShared<FCollisionShape>())
{
}

FSNPhysicsCastNodePreviewTask::~FSNPhysicsCastNodePreviewTask()
{
    if (Shape.IsValid())
        Shape.Reset();
}

void FSNPhysicsCastNodePreviewTask::Enter(const FSNPreviewContext* Context)
{
    UpdateShape(CastChecked<USNPhysicsCastNode>(Context->Node));
}

void FSNPhysicsCastNodePreviewTask::Sample(const FSNPreviewManager* Context, float CurrentTime, float PrevTime)
{
    USNPhysicsCastNode* Node = CastChecked<USNPhysicsCastNode>(Context->PreviewContext->Node);
    UpdateShape(Node);
    UWorld* World = Context->PreviewContext->Actor ? Context->PreviewContext->Actor->GetWorld() : Context->PreviewContext->World.Get();
    USkeletalMeshComponent* SkeletalMeshComponent = Context->PreviewContext->Actor ? Context->PreviewContext->Actor->GetComponentByClass<USkeletalMeshComponent>() : nullptr;

    FVector Start = Node->Start;
    if (!Node->bIsFollowActorLocation || !Context->PreviewContext->Actor)
    {
        Start = Node->Start;
    }
    else
    {
        if (!Node->SocketName.IsEmpty() && SkeletalMeshComponent)
        {
            Start = SkeletalMeshComponent->GetSocketLocation(FName(Node->SocketName));
        }
        else
        {
            Start = Context->PreviewContext->Actor ? Context->PreviewContext->Actor->GetActorLocation() : Node->Start;
        }
    }

    Start += Node->LocationOffset;
    TArray<AActor*> IgnoreActors = {};
    if (Context->PreviewContext->Actor)
        IgnoreActors.Add(Context->PreviewContext->Actor);

    FHitResult Result;
    TArray<FHitResult> Results;
    constexpr ETraceTypeQuery Query = ETraceTypeQuery::TraceTypeQuery1;

    FRotator Orientation = Node->Orientation;
    if (!Node->bIsFollowActorRotation || !Context->PreviewContext->Actor)
    {
        Orientation = Node->Orientation;
    }
    else
    {
        if (!Node->SocketName.IsEmpty() && SkeletalMeshComponent && SkeletalMeshComponent->GetSkinnedAsset())
        {
            // FRotator Rotator = SkeletalMeshComponent->GetSocketRotation(FName(Node->SocketName));
            Orientation = SkeletalMeshComponent->GetSocketRotation(FName(Node->SocketName));
        }
        else
        {
            Orientation = Context->PreviewContext->Actor ? Context->PreviewContext->Actor->GetActorRotation() : Node->Orientation;
        }
    }

    Orientation = Orientation + Node->OrientationOffset;
    FVector End = Start + Orientation.Vector() * Node->CastLength;
    constexpr EDrawDebugTrace::Type DebugTrace = EDrawDebugTrace::ForDuration;
    constexpr int DrawDebugDuration = 5.f;

    bool bIsHit = Node->bIsSingleCast ? World->SweepSingleByChannel(Result, Start, End, Orientation.Quaternion(), Node->Channel, *Shape) :
                                        World->SweepMultiByChannel(Results, Start, End, Orientation.Quaternion(), Node->Channel, *Shape);

    switch (Node->Shape)
    {
    case ESNTraceShapeType::Line:
        if (Node->bIsSingleCast)
            DrawDebugLineTraceSingle(World, Start, End, DebugTrace, bIsHit, Result, FColor::Green, FColor::Red, DrawDebugDuration);
        else
            DrawDebugLineTraceMulti(World, Start, End, DebugTrace, bIsHit, Results, FColor::Green, FColor::Red, DrawDebugDuration);
        // DrawDebugDirectionalArrow(World, Start, End, 5.f, FColor::Green, false, -1, 0, 0);
        break;
    case ESNTraceShapeType::Box:
        if (Node->bIsSingleCast)
            DrawDebugBoxTraceSingle(World, Start, End, Node->HalfExtent, Orientation, DebugTrace, bIsHit, Result, FColor::Green, FColor::Red, DrawDebugDuration);
        else
            DrawDebugBoxTraceMulti(World, Start, End, Node->HalfExtent, Orientation, DebugTrace, bIsHit, Results, FColor::Green, FColor::Red, DrawDebugDuration);
        break;
    case ESNTraceShapeType::Sphere:
        if (Node->bIsSingleCast)
            DrawDebugSphereTraceSingle(World, Start, End, Node->Radius, DebugTrace, bIsHit, Result, FColor::Green, FColor::Red, DrawDebugDuration);
        else
            DrawDebugSphereTraceMulti(World, Start, End, Node->Radius, DebugTrace, bIsHit, Results, FColor::Green, FColor::Red, DrawDebugDuration);
        break;
    case ESNTraceShapeType::Capsule:
        if (Node->bIsSingleCast)
            DrawDebugCapsuleTraceSingle(World, Start, End, Node->Radius, Node->HalfHeight, Orientation, DebugTrace, bIsHit, Result, FColor::Green, FColor::Red, DrawDebugDuration);
        else
            DrawDebugCapsuleTraceMulti(World, Start, End, Node->Radius, Node->HalfHeight, Orientation, DebugTrace, bIsHit, Results, FColor::Green, FColor::Red, DrawDebugDuration);
        break;
    default: break;
    }

    if (bIsHit)
    {
        if (Node->bIsSingleCast)
        {
            Node->OnActionCastHit.Broadcast(Result);
        }
        else
        {
            for (const auto& Hit : Results)
            {
                Node->OnActionCastHit.Broadcast(Hit);
            }
        }
    }

    if (Node->bIsShowColliderMesh)
        DrawContinueColliderMesh(Node, Context->PreviewContext.Get());
}

void FSNPhysicsCastNodePreviewTask::Exit(const FSNPreviewContext* Context)
{
}

void FSNPhysicsCastNodePreviewTask::UpdateShape(const USNPhysicsCastNode* Node) const
{
    switch (Node->Shape)
    {
    case ESNTraceShapeType::Line:
        Shape->ShapeType = ECollisionShape::Line;
        break;
    case ESNTraceShapeType::Box:
        Shape->ShapeType = ECollisionShape::Box;
        Shape->SetBox(static_cast<FVector3f>(Node->HalfExtent));
        break;
    case ESNTraceShapeType::Sphere:
        Shape->ShapeType = ECollisionShape::Sphere;
        Shape->SetSphere(Node->Radius);
        break;
    case ESNTraceShapeType::Capsule:
        Shape->ShapeType = ECollisionShape::Capsule;
        Shape->SetCapsule(Node->Radius, Node->HalfHeight);
        break;
    default: break;
    }
}

void FSNPhysicsCastNodePreviewTask::DrawContinueColliderMesh(const USNPhysicsCastNode* Node, const FSNPreviewContext* Context)
{
    if (!Node->bIsShowColliderMesh)
        return;

    const USkeletalMeshComponent* SkeletalMeshComponent = Context->Actor ? Context->Actor->GetComponentByClass<USkeletalMeshComponent>() : nullptr;
    if (!SkeletalMeshComponent || !SkeletalMeshComponent->GetSkinnedAsset())
        return;

    const UWorld* World = Context->Actor ? Context->Actor->GetWorld() : Context->World.Get();
    if (!SkeletalMeshComponent->AnimationData.AnimToPlay->IsA(UAnimSequence::StaticClass()))
        return;

    if (!SkeletalMeshComponent->DoesSocketExist(FName(Node->SocketName)))
        return;

    UAnimSequence* Sequence = Cast<UAnimSequence>(SkeletalMeshComponent->AnimationData.AnimToPlay);
    FSkeletonPoseBoneIndex BoneIndex = FSkeletonPoseBoneIndex(SkeletalMeshComponent->GetBoneIndex(FName(Node->SocketName)));

    // TODO: draw time points position
    // Sequence->GetBoneTransform(Transform, BoneIndex, 0.f, false);
}
