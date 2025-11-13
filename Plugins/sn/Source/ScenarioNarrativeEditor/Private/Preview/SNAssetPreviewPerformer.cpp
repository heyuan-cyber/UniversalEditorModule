#include "Preview/SNAssetPreviewPerformer.h"
#include "AnimPreviewInstance.h"
#include "ScenarioNarrativeFunctionLibrary.h"
#include "Animation/DebugSkelMeshComponent.h"
#include "DataStyle/SNAssetStyleRegistry.h"
#include "DataStyle/Node/KeyFrame/SNKeyFrameNodeStyle.h"
#include "Node/SNPerformerNode.h"
#include "Preview/SNPreviewActor.h"
#include "Preview/SNPreviewManager.h"
#include "Preview/SNPreviewContext.h"

FSNAssetPreviewPerformer::FSNAssetPreviewPerformer(TWeakPtr<FSNPreviewContext> PreviewContext, float* InCurrentTime):
    PreviewContext(PreviewContext),
    PerformerPool(MakeShared<FPerformerPool>(PreviewContext.Pin()->World)),
    TimelineCurrentTime(InCurrentTime)
{
    RegistryToManager(USNPerformerNode)
}

FSNAssetPreviewPerformer::~FSNAssetPreviewPerformer()
{
}

void FSNAssetPreviewPerformer::SpawnPerformer(TObjectPtr<USNPerformerNode> Node)
{
    if(!Node->Animation && !Node->PreviewSkeleton) return;

    PerformerPool->GetOne(Node)->OnPostEditMove = [this](TObjectPtr<ASNPreviewActor> Performer)
    {
        this->OnPostEditorMove(Performer);
    };
}

void FSNAssetPreviewPerformer::RecylePerformer(TObjectPtr<USNPerformerNode> Node)
{
    PerformerPool->BackOne(Node);
}

/* 添加关键帧的外部接口 */
void FSNAssetPreviewPerformer::CreateAndShowKeyFrame(TObjectPtr<USNNodeBase> Node, FSNKeyFrameNodeStyle* Style)
{
    CreateAndShowKeyFrame(PerformerPool->SearchActivePerformer(Cast<USNPerformerNode>(Node)), Cast<USNPerformerNode>(Node),  Style);
}

/* 添加关键帧的具体实现 */
void FSNAssetPreviewPerformer::CreateAndShowKeyFrame(ASNPreviewActor* Performer, TObjectPtr<USNPerformerNode> Node, FSNKeyFrameNodeStyle* Style)
{
    UDebugSkelMeshComponent* SMC = Performer->GetComponentByClass<UDebugSkelMeshComponent>();
    if (UAnimSingleNodeInstance* AnimSingleNode = Cast<UAnimSingleNodeInstance>(SMC->PreviewInstance))
    {
        if (UAnimSequenceBase* Anim = Cast<UAnimSequenceBase>(AnimSingleNode->GetCurrentAsset()))
        {
            Style->ShowAddedKeyFrame(CreateKeyFrame(Node, RemoveRootTransform(AnimSingleNode, Anim, Performer, Node)));
        }
    }
}

/* 创建关键帧数据体 */
USNKeyFrameWrapper* FSNAssetPreviewPerformer::CreateKeyFrame(TObjectPtr<USNNodeBase> Node, FTransform InitTransform)
{
    ISNKeyFrameInterface* KeyFrameProvider = Cast<ISNKeyFrameInterface>(Node);
    if (KeyFrameProvider)
    {
        // 通过接口访问关键帧
        TArray<FSNKeyFrame>& KeyFrames = KeyFrameProvider->GetKeyFrames();
        FSNKeyFrame NewKeyFrame;
        NewKeyFrame.LocalTime = GetTimelineCurrentTime() - Node->BeginTime;
        NewKeyFrame.Transform = InitTransform;

        // 找到插入位置
        int32 InsertIndex = 0;
        for (; InsertIndex < KeyFrames.Num(); ++InsertIndex)
        {
            if (KeyFrames[InsertIndex].LocalTime > NewKeyFrame.LocalTime)
            {
                break;
            }
        }
        KeyFrames.Insert(NewKeyFrame, InsertIndex);

        // 选中并显示新增的关键帧
        USNKeyFrameWrapper* Wrapper = FScenarioNarrativeFunctionLibrary::CreateTransientObject<USNKeyFrameWrapper>();
        Wrapper->KeyFrame = NewKeyFrame;
        Wrapper->KeyFrameId = InsertIndex;
        Wrapper->SourceNode = Node;
        return Wrapper;
    }
    return nullptr;
}

/* 去除根运动后的Transform */
FTransform FSNAssetPreviewPerformer::RemoveRootTransform(UAnimSingleNodeInstance* AnimSingleNode, UAnimSequenceBase* Anim, ASNPreviewActor* Performer, USNPerformerNode* Node)
{
    if (Node->bActiveLoopMode)
    {
        return Performer->GetActorTransform();
    }
    else
    {
        float CurrentTime = AnimSingleNode->GetCurrentTime();
        FTransform RootTransform = Anim->ExtractRootMotionFromRange(0, CurrentTime);
        // 减去 Root Motion 的Transform
        FTransform CurrentTransform = Performer->GetActorTransform();
        return RootTransform.Inverse() * CurrentTransform;
    }
}

/* 更新初始偏移Transform的接口 */
void FSNAssetPreviewPerformer::UpdateActorInitialTransofrm(ASNPreviewActor* Performer, TObjectPtr<USNPerformerNode> Node)
{
    UpdateInitialTransform(Performer, Node);
}

/* 更新初始偏移Transform的具体实现 */
void FSNAssetPreviewPerformer::UpdateInitialTransform(ASNPreviewActor* Performer, TObjectPtr<USNPerformerNode> Node)
{
    if (Node->bActiveLoopMode)
    {
        Node->InitialTransform = Performer->GetActorTransform();
    }
    else
    {
        // 设置网格体初始位置 ：去除当前根运动位移
        UDebugSkelMeshComponent* SMC = Performer->GetComponentByClass<UDebugSkelMeshComponent>();
        if (UAnimSingleNodeInstance* AnimSingleNode = Cast<UAnimSingleNodeInstance>(SMC->PreviewInstance))
        {
            if (UAnimSequenceBase* Anim = Cast<UAnimSequenceBase>(AnimSingleNode->GetCurrentAsset()))
            {
                float CurrentTime = AnimSingleNode->GetCurrentTime();
                FTransform RootTransform = Anim->ExtractRootMotionFromRange(0, CurrentTime);
                // 减去 Root Motion 的Transform
                FTransform CurrentTransform = Performer->GetActorTransform();
                FTransform FinalTransform = RootTransform.Inverse() * CurrentTransform;
                Node->InitialTransform = FinalTransform;
            }
        }
    }
}

/* 更新指定关键帧的Transform */
void FSNAssetPreviewPerformer::UpdateKeyFrameTransform(ASNPreviewActor* Performer, TObjectPtr<USNPerformerNode> Node, int32 Index)
{
        // 设置网格体初始位置 ：去除当前根运动位移
        UDebugSkelMeshComponent* SMC = Performer->GetComponentByClass<UDebugSkelMeshComponent>();
        if (UAnimSingleNodeInstance* AnimSingleNode = Cast<UAnimSingleNodeInstance>(SMC->PreviewInstance))
        {
            if (UAnimSequenceBase* Anim = Cast<UAnimSequenceBase>(AnimSingleNode->GetCurrentAsset()))
            {
                Node->GetKeyFrames()[Index].Transform = RemoveRootTransform(AnimSingleNode, Anim, Performer, Node);
            }
        }
}

/* 更新关键帧的Transform */
void FSNAssetPreviewPerformer::UpdateActorKeyTransform(ASNPreviewActor* Performer, TObjectPtr<USNPerformerNode> Node)
{
    FSNKeyFrameNodeStyle* Style = static_cast<FSNKeyFrameNodeStyle*>(FSNAssetStyleRegistry::Get().GetNodeStyle(Node->GetClass()));
    // 选中关键帧时，更新当前关键帧
    if (int32 KeyIndex = Style->HasSelectKeyFrame(Node); KeyIndex != -1)
    {
        UpdateKeyFrameTransform(Performer, Node, KeyIndex);
    }
    // 未选中关键帧，自动插帧
    else
    {
        CreateAndShowKeyFrame(Performer, Node, Style);
    }
}

// Gizmo Move Post Handle Event
void FSNAssetPreviewPerformer::OnPostEditorMove(ASNPreviewActor* Performer)
{
    if (auto Node = PerformerPool->SearchOne(Performer))
    {
        // 手动创建关键帧后， 自动更新关键帧数据/自动插帧
        if(Node->HasKeyFrame())
        {
            UpdateActorKeyTransform(Performer, Node);
        }
        // 无关键帧时， 只更新初始偏移
        else
        {
            UpdateActorInitialTransofrm(Performer, Node);
        }
    }
}

FPerformerPool::FPerformerPool(UWorld* World) :
    EditorWorld(World)
{
}

FPerformerPool::~FPerformerPool()
{
    CleanUp();
}

void FPerformerPool::BackOne(TObjectPtr<USNPerformerNode> Node)
{
    if (ActivePerformer.Find(Node))
    {
        TObjectPtr<ASNPreviewActor> RecycledPerformer = ActivePerformer[Node];
        ActivePerformer.Remove(Node);
        ResetOne(RecycledPerformer);
        UnActivePerformer.Push(RecycledPerformer);
    }
}

void FPerformerPool::ResetOne(TObjectPtr<USNPerformerNode> Node)
{
    if (ActivePerformer.Find(Node))
    {
        ResetOne(ActivePerformer[Node]);
    }
}

void FPerformerPool::ResetOne(TObjectPtr<ASNPreviewActor> RecycledPerformer)
{
    if(UDebugSkelMeshComponent* SMC = RecycledPerformer->GetComponentByClass<UDebugSkelMeshComponent>())
    {
        SMC->PreviewInstance->SetPlaying(false);
        SMC->PreviewInstance->SetAnimationAsset(nullptr);
        SMC->SetSkeletalMesh(nullptr);
        RecycledPerformer->SetActorTickEnabled(false);
        RecycledPerformer->SetIsTemporarilyHiddenInEditor(true);
        RecycledPerformer->SetActorEnableCollision(false);
        RecycledPerformer->SetActorTransform(FTransform::Identity);
    }
}

void FPerformerPool::InitOne(TObjectPtr<ASNPreviewActor>Performer, TObjectPtr<USNPerformerNode> Node)
{
    UDebugSkelMeshComponent* SMC = nullptr;
    if (!Performer->FindComponentByClass<UDebugSkelMeshComponent>())
    {
        SMC = NewObject<UDebugSkelMeshComponent>(Performer);
        Performer->AddInstanceComponent(SMC); // 告诉 Actor 这是实例组件, 使用AddComponent会导致编辑器为其创建CDO，在细节面板更新Transform就会崩...
        SMC->RegisterComponent();
        Performer->SetRootComponent(SMC);
    }
    else
    {
        SMC = Performer->GetComponentByClass<UDebugSkelMeshComponent>();
    }

    SMC->SetAnimationMode(EAnimationMode::Type::AnimationSingleNode);
    SMC->SetSkeletalMesh(Node->PreviewSkeleton != nullptr ? Node->PreviewSkeleton.Get() : Node->Animation->GetSkeleton()->GetPreviewMesh());
    SMC->InitAnim(true);
    SMC->SetComponentTickEnabled(true);
    if(Node->Animation)
    {
        SMC->EnablePreview(true, Node->Animation);
        SMC->PreviewInstance->SetPlaying(false);
        SMC->PreviewInstance->SetLooping(false);
        SMC->PreviewInstance->SetPosition(Node->Animation->GetTimeAtFrame(0));
    }
    Performer->SceneComponent = SMC;
    Performer->SetRootComponent(SMC);
    Performer->SetActorTransform(Node->InitialTransform);
    Performer->SetIsTemporarilyHiddenInEditor(false);
    Performer->SetActorTickEnabled(true);
    Performer->SetActorEnableCollision(true);
}

TObjectPtr<ASNPreviewActor> FPerformerPool::GetOne(TObjectPtr<USNPerformerNode> Node)
{
    TObjectPtr<ASNPreviewActor> Performer;

    Performer = UnActivePerformer.Num()
                ? UnActivePerformer.Pop()
                : TObjectPtr<ASNPreviewActor>(EditorWorld->SpawnActor<ASNPreviewActor>());
    InitOne(Performer, Node);
    ActivePerformer.Add(Node, Performer);
    return Performer;
}

TObjectPtr<USNPerformerNode> FPerformerPool::SearchOne(TObjectPtr<ASNPreviewActor> Target)
{
    for (const auto& Pair : ActivePerformer)
    {
        if (Pair.Value == Target)
        {
            return Pair.Key;
        }
    }
    return nullptr;
}

TObjectPtr<ASNPreviewActor> FPerformerPool::SearchActivePerformer(TObjectPtr<USNPerformerNode> Node)
{
    if(!ActivePerformer.Find(Node)) return nullptr;
    return ActivePerformer[Node];
}

void FPerformerPool::CleanUp()
{
    for(auto& Pair : ActivePerformer)
    {
        Pair.Value->Destroy();
    }
    for(auto& Actor : UnActivePerformer)
    {
        Actor->Destroy();
    }
}
