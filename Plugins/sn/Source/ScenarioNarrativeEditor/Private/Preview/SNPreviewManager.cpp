#include "Preview/SNPreviewManager.h"
#include "Node/SNAnimationNode.h"
#include "Preview/SNAssetPreviewBase.h"
#include "Preview/SNAssetPreviewPerformer.h"
#include "Sequencer/SNSequencer.h"
#include "Preview//SNPreviewContext.h"
#include "Task/SNPreviewTaskBase.h"
#include "Task/SNPreviewTaskRegistry.h"
#include <EditorEngine/Setting/SNEditorSetting.h>

#include "DataStyle/Track/SNCameraTrackStyle.h"
#include "PreviewTask/SNSplineCameraNodePreviewTask.h"


TMap<UClass*, FSNAssetPreviewBase*>  FSNPreviewManager::NodeToManager = {};

FSNPreviewManager::FSNPreviewManager():
    PreviewContext(MakeShared<FSNPreviewContext>()),
    State(ESNPreviewPlayerState::Paused),
    CurrentTime(0.f),
    LastSampleTime(0.f),
    LastSampleNodes(),
    AccumulatedTime(0.f)
{
    USNEditorSetting* Settings = GetMutableDefault<USNEditorSetting>();
    TickInterval = 1.f / Settings->FrameRate;
}

FSNPreviewManager::~FSNPreviewManager()
{
    if (PreviewContext->AssetSequence)
    {
        for (const auto& Track : PreviewContext->AssetSequence->Tracks)
        {
            if (!Track)
                continue;

            if (FSNPreviewTaskBase* TrackPreviewTask = FSNPreviewTaskRegistry::Get().GetPreviewTask(Track))
            {
                PreviewContext->Track = Track;
                TrackPreviewTask->Close(PreviewContext.Get());
            }

            for (const auto& Node : Track->Nodes)
            {
                if (!Node)
                    continue;

                if (FSNPreviewTaskBase* NodePreviewTask = FSNPreviewTaskRegistry::Get().GetPreviewTask(Node))
                {
                    PreviewContext->Node = Node;
                    NodePreviewTask->Close(PreviewContext.Get());
                }
            }
        }
    }
}

void FSNPreviewManager::Tick(float DeltaTime)
{
    // 模拟帧率控制
    AccumulatedTime += DeltaTime;

    if (AccumulatedTime >= TickInterval)
    {
        // 真正执行的逻辑
        if (State == ESNPreviewPlayerState::Stopped)
        {
            return;
        }

        SampleAtTime(CurrentTime);

        if (State == ESNPreviewPlayerState::Playing)
        {
            CurrentTime += AccumulatedTime;
        }

        // 减去一个 TickInterval，防止抖动
        AccumulatedTime -= TickInterval;
    }
}

void FSNPreviewManager::JumpTo(float NextTime)
{
    if (CurrentTime == NextTime)
    {
        return;
    }

    if (State == ESNPreviewPlayerState::Paused)
    {
        SampleAtTime(NextTime);
    }

    CurrentTime = NextTime;
}

void FSNPreviewManager::SampleAtTime(float Time)
{
    TSet<TObjectPtr<USNNodeBase>> SampleNodes;

    for (const auto& Track : PreviewContext->AssetSequence->Tracks)
    {
        if (!Track)
        {
            continue;
        }

        if (FSNPreviewTaskBase* TrackPreviewTask = FSNPreviewTaskRegistry::Get().GetPreviewTask(Track))
        {
            TrackPreviewTask->Sample(this, Time, LastSampleTime);
        }

        for (const auto& Node : Track->Nodes)
        {
            if (!Node)
            {
                continue;
            }

            if (Node->BeginTime <= Time && Node->EndTime >= Time)
            {
                SampleNodes.Add(Node);
            }
        }
    }

    // exit first
    for (const auto& Node : LastSampleNodes)
    {
        if (FSNPreviewManager::NodeToManager.Find(Node.GetClass()))
        {
            if (!SampleNodes.Contains(Node))
            {
                PreviewContext->Node = Node;
                NodeToManager[Node.GetClass()]->Exit(PreviewContext.Get());
            }
        }
    }

    for (const auto& Node : SampleNodes)
    {
        if (FSNPreviewManager::NodeToManager.Find(Node.GetClass()))
        {
            PreviewContext->Node = Node;
            if (!LastSampleNodes.Contains(Node))
            {
                 NodeToManager[Node.GetClass()]->Enter(PreviewContext.Get());
            }
            NodeToManager[Node.GetClass()]->Sample(this, Time, LastSampleTime);
        }
    }

    LastSampleNodes = SampleNodes;
    LastSampleTime = Time;
}
