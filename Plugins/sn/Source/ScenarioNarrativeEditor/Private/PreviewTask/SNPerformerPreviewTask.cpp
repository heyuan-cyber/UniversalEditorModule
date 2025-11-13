#include "PreviewTask/SNPerformerPreviewTask.h"
#include "AnimPreviewInstance.h"
#include "Animation/DebugSkelMeshComponent.h"
#include "EditorEngine/SNEditorEngine.h"
#include "Preview/SNAssetPreviewPerformer.h"
#include "Preview/SNPreviewActor.h"
#include "Preview/SNPreviewContext.h"

void FSNPerformerPreviewTask::Enter(const FSNPreviewContext* Context)
{
    if(auto Node = Cast<USNPerformerNode>(Context->Node))
    {
        Context->Performer->SpawnPerformer(Node);
    }
}

void FSNPerformerPreviewTask::Sample(const FSNPreviewManager* Context, float CurrentTime, float PrevTime)
{
    if(auto Node = Cast<USNPerformerNode>(Context->PreviewContext->Node))
    {
        if(auto Performer = Context->PreviewContext->Performer->GetCurrentPerformer(Node))
        {
            if(UDebugSkelMeshComponent* SMC = Performer->GetComponentByClass<UDebugSkelMeshComponent>())
            {
                if(Node->Animation && SMC->PreviewInstance->GetAnimSequence() != Node->Animation)
                {
                    SMC->EnablePreview(true, Node->Animation);
                }

                const float BeginTime = Node->Animation->GetTimeAtFrame(0);
                float LocalTime;
                float PrevLocalTime;
                FTransform CurrentTransform = Performer->GetActorTransform();
                FTransform FinalTransform;
                // 处理 时间 和 Transform
                if(Node->bActiveLoopMode)
                {
                    const float AnimLen = Node->Animation->GetPlayLength();
                    int Count = (CurrentTime - Node->BeginTime) / AnimLen;
                    LocalTime = (CurrentTime - Node->BeginTime - Count * AnimLen) * Node->PlayRate + BeginTime;
                    PrevLocalTime = (PrevTime - Node->BeginTime - Count * AnimLen) * Node->PlayRate + BeginTime;
                    FinalTransform = Node->GetTimeInterpTransform(Node->InitialTransform, CurrentTime- Node->BeginTime);
                }
                else
                {
                    LocalTime = (CurrentTime - Node->BeginTime) * Node->PlayRate + BeginTime;
                    PrevLocalTime = (PrevTime - Node->BeginTime) * Node->PlayRate + BeginTime;
                    FinalTransform = Node->GetTimeInterpTransform(Node->InitialTransform, CurrentTime- Node->BeginTime);

                    // 取绝对根运动（动画起点 -> 当前时间），叠加根运动Transform
                    FTransform AbsoluteRM = Node->Animation->ExtractRootMotionFromRange(0.f, LocalTime);
                    FinalTransform = AbsoluteRM * FinalTransform;
                }
                // 更新 Transform
                Performer->SetActorTransform(FinalTransform);
                // 更新 骨骼
                SMC->PreviewInstance->SetPositionWithPreviousTime(LocalTime, PrevLocalTime);
                // 更新 Gizmo
                USNEditorEngine::Get()->UpdatePivotLocationForSelection(true);
            }
        }
    }
}

void FSNPerformerPreviewTask::Exit(const FSNPreviewContext* Context)
{
    if(auto Node = Cast<USNPerformerNode>(Context->Node))
    {
        Context->Performer->RecylePerformer(Node);
    }
}
