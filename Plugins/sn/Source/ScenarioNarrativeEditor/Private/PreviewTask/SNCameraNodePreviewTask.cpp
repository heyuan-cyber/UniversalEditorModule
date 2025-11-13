#include "PreviewTask/SNCameraNodePreviewTask.h"
#include "SNAssetBase.h"
#include "Node/SNCameraNode.h"
#include "Preview/SNAssetPreviewCamera.h"
#include "Preview/SNPreviewCamera.h"
#include "Preview/SNPreviewContext.h"

void FSNCameraNodePreviewTask::Enter(const FSNPreviewContext* Context)
{
    USNCameraNode* Node = Cast<USNCameraNode>(Context->Node);
    // FSNAssetPreviewCamera* PreviewActor = Cast<FSNAssetPreviewCamera>(Context->Actor);

}

void FSNCameraNodePreviewTask::Sample(const FSNPreviewManager* Context, float CurrentTime, float PrevTime)
{
    USNCameraNode* Node = Cast<USNCameraNode>(Context->PreviewContext->Node);
    float NodeBeginTime = Node->BeginTime;
    float RelativeTime = CurrentTime - NodeBeginTime;
    TArray<FSNCameraKeyFrame> CameraKeyFrameArray = Node->GetKetFramesByCurrentTime(RelativeTime);
    FSNCameraKeyFrame& BeginKeyFrame = CameraKeyFrameArray[0];
    FSNCameraKeyFrame& EndKeyFrame = CameraKeyFrameArray[1];

    if (!BeginKeyFrame.IsValid() && !EndKeyFrame.IsValid())
    {
        return;
    }

    FVector FinalPosition;
    FRotator FinalRotation;
    float FinalFOV;

    if (!BeginKeyFrame.IsValid())
    {
        FinalPosition = EndKeyFrame.Position;
        FinalRotation = EndKeyFrame.Rotation;
        FinalFOV = EndKeyFrame.FOV;
    }
    else if (!EndKeyFrame.IsValid())
    {
        FinalPosition = BeginKeyFrame.Position;
        FinalRotation = BeginKeyFrame.Rotation;
        FinalFOV = BeginKeyFrame.FOV;
    }
    else
    {
        float TotalDuration = EndKeyFrame.Time - BeginKeyFrame.Time;
        float Alpha = (TotalDuration > SMALL_NUMBER)
            ? FMath::Clamp((RelativeTime - BeginKeyFrame.Time) / TotalDuration, 0.0f, 1.0f)
            : 0.0f;

        FinalPosition = FMath::Lerp(
            BeginKeyFrame.Position,
            EndKeyFrame.Position,
            Alpha
        );

        FQuat StartQuat = BeginKeyFrame.Rotation.Quaternion();
        FQuat EndQuat = EndKeyFrame.Rotation.Quaternion();

        FQuat InterpolatedQuat = FQuat::Slerp(StartQuat, EndQuat, Alpha);
        FinalRotation = InterpolatedQuat.Rotator();

        FinalFOV = FMath::Lerp(
            BeginKeyFrame.FOV,
            EndKeyFrame.FOV,
            Alpha
        );
    }

    FVector FocusTargetPosition;
    bool bHasFocusTarget = false;

    if (Node->FocusMode == ESNCameraFocusMode::Actor)
    {
        FocusTargetPosition = Node->FocusTargetActor->GetActorLocation();
        bHasFocusTarget = true;
    }
    else if (Node->FocusMode == ESNCameraFocusMode::Location)
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

    Context->PreviewContext->Camera->PreviewCamera->SetActorLocation(FinalPosition);
    Context->PreviewContext->Camera->PreviewCamera->SetActorRotation(FinalRotation);
}

void FSNCameraNodePreviewTask::Exit(const FSNPreviewContext* Context)
{

}
