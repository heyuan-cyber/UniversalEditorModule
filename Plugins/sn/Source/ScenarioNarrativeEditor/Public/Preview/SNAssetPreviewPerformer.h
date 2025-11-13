#pragma once
#include "SNAssetPreviewBase.h"
#include "DataStyle/Node/KeyFrame/SNKeyFrameNodeStyle.h"
#include "Node/KeyFrame/SNKeyFrame.h"

class ASNPreviewActor;
class USNPerformerNode;
class FSNActorPool;

class FPerformerPool
{
public:
    FPerformerPool(UWorld* World);

    ~FPerformerPool();

    TObjectPtr<UWorld> EditorWorld;

    TMap<TObjectPtr<USNPerformerNode>, TObjectPtr<ASNPreviewActor>> ActivePerformer;

    TArray<TObjectPtr<ASNPreviewActor>> UnActivePerformer;

    void BackOne(TObjectPtr<USNPerformerNode> Node);

    void ResetOne(TObjectPtr<USNPerformerNode> Node);

    void ResetOne(TObjectPtr<ASNPreviewActor> RecycledPerformer);

    void InitOne(TObjectPtr<ASNPreviewActor> Performer, TObjectPtr<USNPerformerNode> Node);

    TObjectPtr<ASNPreviewActor> GetOne(TObjectPtr<USNPerformerNode>);

    TObjectPtr<USNPerformerNode> SearchOne(TObjectPtr<ASNPreviewActor>);

    TObjectPtr<ASNPreviewActor> SearchActivePerformer(TObjectPtr<USNPerformerNode> Node);

    void CleanUp();
};

class FSNAssetPreviewPerformer : FSNAssetPreviewBase
{
public:
    FSNAssetPreviewPerformer(TWeakPtr<FSNPreviewContext>  PreviewContext, float* InCurrentTime);

    virtual ~FSNAssetPreviewPerformer();

private:
    TWeakPtr<FSNPreviewContext>  PreviewContext;

    TSharedPtr<FPerformerPool> PerformerPool;

    float* TimelineCurrentTime;

public:
    TObjectPtr<ASNPreviewActor> GetCurrentPerformer(TObjectPtr<USNPerformerNode> Node) { return PerformerPool->ActivePerformer.Find(Node) ? PerformerPool->ActivePerformer[Node] : nullptr; }

    void SpawnPerformer(TObjectPtr<USNPerformerNode> Node);

    void RecylePerformer(TObjectPtr<USNPerformerNode> Node);

    void CreateAndShowKeyFrame(TObjectPtr<USNNodeBase> Node, FSNKeyFrameNodeStyle* Style);

    USNKeyFrameWrapper* CreateKeyFrame(TObjectPtr<USNNodeBase> Node, FTransform InitTransform);

    void UpdateInitialTransform(ASNPreviewActor* Performer, TObjectPtr<USNPerformerNode> Node);

    FTransform RemoveRootTransform(UAnimSingleNodeInstance* AnimSingleNode, UAnimSequenceBase* Anim, ASNPreviewActor* Performer, USNPerformerNode* Node);

    void UpdateKeyFrameTransform(ASNPreviewActor* Performer, TObjectPtr<USNPerformerNode> Node, int32 Index);

    void UpdateActorInitialTransofrm(ASNPreviewActor* Performer, TObjectPtr<USNPerformerNode> Node);

    void CreateAndShowKeyFrame(ASNPreviewActor* Performer, TObjectPtr<USNPerformerNode> Node,FSNKeyFrameNodeStyle* Style);

    void UpdateActorKeyTransform(ASNPreviewActor* Performer, TObjectPtr<USNPerformerNode> Node);

    void OnPostEditorMove(ASNPreviewActor* Performer);

    float GetTimelineCurrentTime() const { return *TimelineCurrentTime; }
};


