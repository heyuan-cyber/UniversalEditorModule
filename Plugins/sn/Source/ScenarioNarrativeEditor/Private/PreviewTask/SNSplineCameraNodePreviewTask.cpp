#include "PreviewTask/SNSplineCameraNodePreviewTask.h"
#include "SNAssetBase.h"
#include "Node/SNSplineCameraNode.h"
#include "Preview/SNAssetPreviewCamera.h"
#include "Preview/SNPreviewCamera.h"
#include "Preview/SNPreviewContext.h"
#include "Components/SplineComponent.h"
#include "Preview/SNAssetPreviewSpline.h"
#include "Preview/SNPreviewSpline.h"

void FSNSplineCameraNodePreviewTask::Enter(const FSNPreviewContext* Context)
{
    USNSplineCameraNode* Node = Cast<USNSplineCameraNode>(Context->Node);
    if (!Node || !Node->SplineComponent.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("SplineCameraNodePreviewTask: Invalid node or spline component"));
        return;
    }

    // 初始化缓存
    CachedPrevKeyFrameIndex = -1;
    CachedNextKeyFrameIndex = -1;

    UE_LOG(LogTemp, Log, TEXT("SplineCameraNodePreviewTask: Entered for node %s"), *Node->GetName());
}

void FSNSplineCameraNodePreviewTask::Sample(const FSNPreviewManager* Context, float CurrentTime, float PrevTime)
{
    USNSplineCameraNode* Node = Cast<USNSplineCameraNode>(Context->PreviewContext->Node);
    if (!Node)
    {
        return;
    }

    ASNPreviewSpline* PreviewSpline = Context->PreviewContext->GetResource<FSNAssetPreviewSpline>()->GetSplineForNode(Cast<USNSplineCameraNode>(Node));
    if (!PreviewSpline || !IsValid(PreviewSpline->SplineComponent))
    {
        return;
    }

    float NodeBeginTime = Node->BeginTime;
    float RelativeTime = CurrentTime - NodeBeginTime;

    // 确保关键帧按时间排序
    Node->SortKeyFramesByTime();
    TArray<FSNSplineCameraKeyFrame> KeyFrames = Node->KeyFrames;

    // 处理边界情况 - 没有有效关键帧
    if (KeyFrames.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("SplineCameraNodePreviewTask: No valid keyframes"));
        return;
    }

    FVector FinalPosition;
    FRotator FinalRotation;

    // 如果只有一个关键帧或当前时间在第一个关键帧之前
    if (KeyFrames.Num() == 1 || RelativeTime <= KeyFrames[0].Time)
    {
        const FSNSplineCameraKeyFrame& KeyFrame = KeyFrames[0];
        int32 ControlPointIndex = KeyFrame.PointInputKey;

        FinalPosition = PreviewSpline->SplineComponent->GetLocationAtSplinePoint(ControlPointIndex, ESplineCoordinateSpace::World);
        FinalRotation = PreviewSpline->SplineComponent->GetRotationAtSplinePoint(ControlPointIndex, ESplineCoordinateSpace::World);

        CachedPrevKeyFrameIndex = 0;
        CachedNextKeyFrameIndex = -1;
    }
    // 如果当前时间在最后一个关键帧之后
    else if (RelativeTime >= KeyFrames.Last().Time)
    {
        const FSNSplineCameraKeyFrame& KeyFrame = KeyFrames.Last();
        int32 LastIndex = KeyFrames.Num() - 1;
        int32 ControlPointIndex = KeyFrame.PointInputKey;

        FinalPosition = PreviewSpline->SplineComponent->GetLocationAtSplinePoint(ControlPointIndex, ESplineCoordinateSpace::World);
        FinalRotation = PreviewSpline->SplineComponent->GetRotationAtSplinePoint(ControlPointIndex, ESplineCoordinateSpace::World);

        CachedPrevKeyFrameIndex = LastIndex;
        CachedNextKeyFrameIndex = -1;
    }
    else
    {
        TArray<FSNSplineCameraKeyFrame> CameraKeyFrameArray = Node->GetKeyFramesByCurrentTime(RelativeTime);
        int32 PrevKeyFrameIndex = CameraKeyFrameArray[0].PointInputKey;
        int32 NextKeyFrameIndex = CameraKeyFrameArray[1].PointInputKey;

        if (PrevKeyFrameIndex == -1 || NextKeyFrameIndex == -1)
        {
            UE_LOG(LogTemp, Warning, TEXT("SplineCameraNodePreviewTask: Failed to find keyframe interval for time %.2f"), RelativeTime);
            return;
        }

        const FSNSplineCameraKeyFrame& PrevFrame = KeyFrames[PrevKeyFrameIndex];
        const FSNSplineCameraKeyFrame& NextFrame = KeyFrames[NextKeyFrameIndex];

        float TimeRange = NextFrame.Time - PrevFrame.Time;
        float Alpha = (TimeRange > 0.0f) ? FMath::Clamp((RelativeTime - PrevFrame.Time) / TimeRange, 0.0f, 1.0f) : 0.0f;

        float PrevDistance = PreviewSpline->SplineComponent->GetDistanceAlongSplineAtSplinePoint(PrevKeyFrameIndex);
        float NextDistance = PreviewSpline->SplineComponent->GetDistanceAlongSplineAtSplinePoint(NextKeyFrameIndex);
        float InterpolatedDistance = FMath::Lerp(PrevDistance, NextDistance, Alpha);

        FinalPosition = PreviewSpline->SplineComponent->GetLocationAtDistanceAlongSpline(InterpolatedDistance, ESplineCoordinateSpace::World);
        FinalRotation = PreviewSpline->SplineComponent->GetRotationAtDistanceAlongSpline(InterpolatedDistance, ESplineCoordinateSpace::World);

        CachedPrevKeyFrameIndex = PrevKeyFrameIndex;
        CachedNextKeyFrameIndex = NextKeyFrameIndex;
    }

    FVector FocusTargetPosition;
    bool bHasFocusTarget = false;

    if (Node->FocusMode == ESNSplineCameraFocusMode::Actor)
    {
        FocusTargetPosition = Node->FocusTargetActor->GetActorLocation();
        bHasFocusTarget = true;
    }
    else if (Node->FocusMode == ESNSplineCameraFocusMode::Location)
    {
        FocusTargetPosition = Node->FocusTargetPosition;
        bHasFocusTarget = true;
    }

    if (bHasFocusTarget)
    {
        FVector Direction = FocusTargetPosition - FinalPosition;
        FRotator TargetRotation = Direction.Rotation();

        if (Node->bBlendFocusRotation)
        {
            FinalRotation = FQuat::Slerp(
                FinalRotation.Quaternion(),
                TargetRotation.Quaternion(),
                Node->FocusBlendAmount
            ).Rotator();
        }
        else
        {
            FinalRotation = TargetRotation;
        }
    }

    if (Context->PreviewContext->Camera && Context->PreviewContext->Camera->PreviewCamera)
    {
        Context->PreviewContext->Camera->PreviewCamera->SetActorLocation(FinalPosition);
        Context->PreviewContext->Camera->PreviewCamera->SetActorRotation(FinalRotation);
    }
}

void FSNSplineCameraNodePreviewTask::Exit(const FSNPreviewContext* Context)
{
    USNSplineCameraNode* Node = Cast<USNSplineCameraNode>(Context->Node);
    if (Node)
    {
        UE_LOG(LogTemp, Log, TEXT("SplineCameraNodePreviewTask: Exited for node %s"), *Node->GetName());
    }

    // 重置状态
    CachedPrevKeyFrameIndex = -1;
    CachedNextKeyFrameIndex = -1;
}
