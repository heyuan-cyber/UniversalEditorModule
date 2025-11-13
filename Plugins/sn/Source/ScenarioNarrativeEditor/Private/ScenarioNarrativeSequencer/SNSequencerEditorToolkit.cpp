#include "ScenarioNarrativeSequencer/SNSequencerEditorToolkit.h"

#include "EditorModeManager.h"
#include "EditorEngine/SNEditorEngine.h"
#include "SNSequencerContext.h"
#include "ScenarioNarrativeEditorDefines.h"
#include "SceneOutlinerModule.h"
#include "Selection.h"
#include "DataStyle/DetailCustomization/DialogNodeDetailCustomization.h"
#include "Editor/TransBuffer.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Node/SNNodeBase.h"
#include "Node/SNAnimationNode.h"
#include "Node/SNCameraNode.h"
#include "Node/SNDialogNode.h"
#include "Node/SNMontageSetNode.h"
#include "Node/SNPerformerNode.h"
#include "Node/SNSplineCameraNode.h"
#include "Preview/SNPreviewManager.h"
#include "PlaybackController/SequencerPlaybackController.h"
#include "Preview/SNAssetPreviewCamera.h"
#include "Preview/SNAssetPreviewPerformer.h"
#include "Preview/SNAssetPreviewSpline.h"
#include "Preview/SNPreviewActor.h"
#include "Preview/SNPreviewSpline.h"
#include "Preview/UI/SNAssetPreviewHUD.h"
#include "PreviewScene/SNAssetPreviewScene.h"
#include "PreviewScene/Viewport/SNAssetViewport.h"
#include "PreviewScene/Viewport/SNAssetViewportClient.h"
#include "Sequencer/SNSequencer.h"
#include "SequencerEditor/SNNodeWidget.h"
#include "SequencerEditor/SNSequencerWidget.h"
#include "Track/SNAnimationTrack.h"
#include "Track/SNCameraTrack.h"
#include "Widgets/Notifications/SNotificationList.h"

#define LOCTEXT_NAMESPACE "SNSequencerEditorToolkit"

FSNSequencerEditorToolkit::FSNSequencerEditorToolkit()
{
    PreviewManager = MakeShared<FSNPreviewManager>();
    Context = MakeShared<FSNSequencerContext>();
    Context->ToolkitCommands = ToolkitCommands;
    Context->InitializePlaybackController();
    Context->PreviewManager = PreviewManager;
    FEditorDelegates::OnAssetsPreDelete.AddRaw(this, &FSNSequencerEditorToolkit::HandleObjectPendingDelete);
}

FSNSequencerEditorToolkit::~FSNSequencerEditorToolkit()
{
    UE_LOG(LogScenarioNarrativeEditor, Warning, TEXT("seq widget free, ViewportWidget ref count: %d"), ViewportWidget.GetSharedReferenceCount());

    FEditorDelegates::OnAssetsPreDelete.RemoveAll(this);

    Context.Reset();
    PreviewManager.Reset();
    PreviewCamera.Reset();
    PreviewScene.Reset();
    PreviewSpline.Reset();
    ViewportWidget.Reset();
    USNEditorEngine::Shutdown();
}

void FSNSequencerEditorToolkit::InitAssetEditor(const EToolkitMode::Type InMode, const TSharedPtr<IToolkitHost>& InitToolkitHost, const TObjectPtr<USNSequencer>& InAsset)
{
    Target = InAsset;
    PreviewScene = MakePreviewScene();
    Context->PreviewManager->PreviewContext->SetResource(PreviewScene.Get());
    EditorModeManager = MakeShared<FEditorModeTools>();
    FAssetEditorToolkit::InitAssetEditor(InMode, InitToolkitHost, "SNSequencerEditor",
                                         CreateDefaultLayout(), true, true, InAsset);

    UTransBuffer* TransBuffer = CastChecked<UTransBuffer>(GEditor->Trans);
    // TransBuffer->OnTransSNStateChanged().AddRaw(this, &FSNSequencerEditorToolkit::OnTransSNChanged);
    TransBuffer->OnUndo().AddRaw(this, &FSNSequencerEditorToolkit::OnUndo);
    TransBuffer->OnRedo().AddRaw(this, &FSNSequencerEditorToolkit::OnRedo);

    PreviewHUD = MakeShared<FSNAssetPreviewHUD>(PreviewManager->PreviewContext);
    PreviewCamera = MakeShared<FSNAssetPreviewCamera>(PreviewManager->PreviewContext);
    PreviewSpline = MakeShared<FSNAssetPreviewSpline>(PreviewManager->PreviewContext);

    // PreviewManager->InitPreviewContext(Target, ViewportWidget, PreviewCamera, PreviewHUD, PreviewScene->GetWorld());


    PreviewManager->PreviewContext->SetResource(Target);
    PreviewManager->PreviewContext->SetResource(ViewportWidget);
    PreviewManager->PreviewContext->SetResource(PreviewCamera);
    PreviewManager->PreviewContext->SetResource(PreviewHUD);
    PreviewManager->PreviewContext->SetResource(PreviewScene->GetWorld());
    PreviewManager->PreviewContext->SetResource(MakeShared<FSNAssetPreviewPerformer>(PreviewManager->PreviewContext, &PreviewManager->CurrentTime).ToSharedPtr());

    Context->Asset = Target;

    FSNAssetViewportClient* SNAssetViewportClient = ViewportWidget->GetSNViewportClient();
    USNEditorEngine::NewInstance();
    USNEditorEngine* SNEE = USNEditorEngine::Get();
    SNAssetViewportClient->SNEE = SNEE;
    SNEE->ViewportClient = SNAssetViewportClient;
    SNEE->ToolkitHost = ToolkitHost.Pin().Get();
    SNEE->SNEditor = this;
    SNEE->Init();

    Context->PreviewManager->PreviewContext->SetResource(PreviewCamera.Get());
    Context->PreviewManager->PreviewContext->SetResource(PreviewHUD.Get());
    Context->PreviewManager->PreviewContext->SetResource(PreviewSpline.Get());

    UpdatePreviewResource();

    Context->OnSelectChanged.AddLambda([this](const TObjectPtr<USNAssetBase>& InObject)
    {
        if (!InspectorWidget.IsValid())
            return;

        InspectorWidget->SetObject(InObject);
    });

    Context->OnSelectUObjectChanged.AddLambda([this](const TObjectPtr<UObject>& InObject)
    {
        if (!InspectorWidget.IsValid())
            return;

        InspectorWidget->SetObject(InObject);
    });

    Context->OnSaveCurrentUObjectChanged.AddLambda([this]()
    {
        FEditorViewportClient* EditorClient = ViewportWidget->GetViewportClient().Get();
        FSNAssetViewportClient* SNAssetClient = static_cast<FSNAssetViewportClient*>(EditorClient);
        if (SNAssetClient) {
            SNAssetClient->SaveCurrentSelectUObjectData();
        }
    });

    Context->OnNodeCreated.AddRaw(this, &FSNSequencerEditorToolkit::OnNodeCreated);
    Context->OnNodeDeleted.AddRaw(this, &FSNSequencerEditorToolkit::OnNodeDeleted);
    Context->OnTrackDeleted.AddRaw(this, &FSNSequencerEditorToolkit::OnTrackDeleted);

    UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("AssetEditor.SNSequencerEditorToolkit.ToolBar");
    FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("SNSequencerEditor");

    Section.AddEntry(FToolMenuEntry::InitToolBarButton
    (
        TEXT("ExportToJson"),
        FExecuteAction::CreateRaw(this, &FSNSequencerEditorToolkit::ExportAssetToJson),
        INVTEXT(""),
        INVTEXT("导出资源到json文件"),
        FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Toolbar.Export")
    ));

    RegenerateMenusAndToolbars();
}

void FSNSequencerEditorToolkit::Tick(float DeltaTime)
{
    // 计算真实的时间增量（考虑暂停等情况）
    const double CurrentTime = FPlatformTime::Seconds();
    const double RealDeltaTime = LastTickTime > 0 ? CurrentTime - LastTickTime : 0.0;
    LastTickTime = CurrentTime;

    // 更新播放控制器
    if (Context.IsValid() && Context->GetPlaybackController())
    {
        Context->GetPlaybackController()->Tick(static_cast<float>(RealDeltaTime));
    }
}

TStatId FSNSequencerEditorToolkit::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(FSNSequencerEditorToolkit, STATGROUP_Tickables);
}

bool FSNSequencerEditorToolkit::IsTickable() const
{
    return FSlateApplication::IsInitialized();
}

TSharedRef<FTabManager::FLayout> FSNSequencerEditorToolkit::CreateDefaultLayout() const
{
    return FTabManager::NewLayout("DefaultSNSequencerEditorLayout_v1.0")
    ->AddArea
    (
        FTabManager::NewPrimaryArea()
        ->SetOrientation(Orient_Horizontal)
        ->Split
        (
            FTabManager::NewSplitter()
            ->SetSizeCoefficient(0.8f)
            ->SetOrientation(Orient_Vertical)
            ->Split
            (
                FTabManager::NewStack()
                ->SetSizeCoefficient(0.6f)
                ->AddTab("ViewportTab", ETabState::OpenedTab)
            )
            ->Split
            (
                FTabManager::NewStack()
                ->SetSizeCoefficient(0.4f)
                ->AddTab("SNSequencerTab", ETabState::OpenedTab)
            )
        )
        ->Split
        (
            FTabManager::NewSplitter()
            ->SetSizeCoefficient(0.2f)
            ->SetOrientation(Orient_Vertical)
            ->Split
            (
            FTabManager::NewStack()
                ->SetSizeCoefficient(0.3f)
                ->AddTab("OutlineTab", ETabState::OpenedTab)
            )
            ->Split
            (
                FTabManager::NewStack()
                ->SetSizeCoefficient(0.7f)
                ->AddTab("InspectorTab", ETabState::OpenedTab)
            )

        )
    );
}

void FSNSequencerEditorToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
    FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

    auto MenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(INVTEXT("SN Sequencer Editor"));

    InTabManager->RegisterTabSpawner("SNSequencerTab", FOnSpawnTab::CreateLambda([this](const FSpawnTabArgs& InArgs)
    {
        return SNew(SDockTab)
        [
            SAssignNew(SNSequencerWidget, SSNSequencerWidget)
            .Asset(Target)
            .Context(Context)
        ];
    }))
    .SetDisplayName(INVTEXT("Timeline"))
    .SetGroup(MenuCategory);

    // Outline View named Outline
    InTabManager->RegisterTabSpawner("OutlineTab", FOnSpawnTab::CreateLambda([this](const FSpawnTabArgs& InArgs)
    {
        // 获取SceneOutliner模块
        FSceneOutlinerModule& SceneOutlinerModule = FModuleManager::LoadModuleChecked<FSceneOutlinerModule>("SceneOutliner");

        // 初始化选项
        FSceneOutlinerInitializationOptions InitOptions;
        {
            InitOptions.bShowHeaderRow = true;
            InitOptions.bShowSearchBox = true;
            InitOptions.bShowCreateNewFolder = false;
            InitOptions.bFocusSearchBoxWhenOpened = true;
        }

        // 创建SceneOutliner
        OutlinerWidget = SceneOutlinerModule.CreateActorPicker(InitOptions,
            FOnActorPicked::CreateSP(this, &FSNSequencerEditorToolkit::OnActorPicked),
            PreviewScene->GetWorld(), false);

        return SNew(SDockTab)
        [
            OutlinerWidget.ToSharedRef()
        ];
    }))
    .SetDisplayName(INVTEXT("Outline"))
    .SetGroup(MenuCategory);

    // Details View named inspector
    InTabManager->RegisterTabSpawner("InspectorTab", FOnSpawnTab::CreateLambda([this](const FSpawnTabArgs& InArgs)
    {
        FPropertyEditorModule& PropertyEditorModule = FModuleManager::Get().LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
        FDetailsViewArgs DetailsViewArgs;
        DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;
        DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
        DetailsViewArgs.NotifyHook = this;

        // 启用自定义布局
        DetailsViewArgs.bCustomNameAreaLocation = true;
        DetailsViewArgs.bCustomFilterAreaLocation = true;
        InspectorWidget = PropertyEditorModule.CreateDetailView(DetailsViewArgs);

        // 创建自定义细节面板
        TSharedPtr<FDialogNodeDetailCustomization> Customization = FDialogNodeDetailCustomization::MakeInstance();

       // 注册自定义细节面板
       InspectorWidget->RegisterInstancedCustomPropertyLayout(
           USNDialogNode::StaticClass(),
           FOnGetDetailCustomizationInstance::CreateLambda([]() -> TSharedRef<IDetailCustomization> {
               return FDialogNodeDetailCustomization::MakeInstance();
           })
       );

        return SNew(SDockTab)
        [
            InspectorWidget.ToSharedRef()
        ];
    }))
    .SetDisplayName(INVTEXT("Inspector"))
    .SetGroup(MenuCategory);

    InTabManager->RegisterTabSpawner("ViewportTab", FOnSpawnTab::CreateLambda([this](const FSpawnTabArgs& InArgs)
    {
        return SNew(SDockTab)
        .OnCanCloseTab(SDockTab::FCanCloseTab::CreateLambda([]
        {
            FNotificationInfo NotifyInfo(INVTEXT("不允许关闭Scene视图！"));
            NotifyInfo.bUseLargeFont = true;
            FSlateNotificationManager::Get().AddNotification(NotifyInfo);
            return false;
        }))
        [
            SAssignNew(ViewportWidget, SSNAssetViewport)
            .PreviewScene(PreviewScene)
            .Context(Context)
            .InspectorWidget(InspectorWidget)
            .InModeTools(EditorModeManager)
        ];
    }))
    .SetDisplayName(INVTEXT("Scene"))
    .SetGroup(MenuCategory);

    InTabManager->RegisterTabSpawner("DetailsTab", FOnSpawnTab::CreateLambda([this](const FSpawnTabArgs& Args)
    {
        FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
        FDetailsViewArgs DetailsViewArgs;
        DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
        TSharedRef<IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
        DetailsView->SetObject(Target);
        return SNew(SDockTab)
        [
            DetailsView
        ];
    }))
    .SetDisplayName(INVTEXT("Debug"))
    .SetGroup(MenuCategory);
}

void FSNSequencerEditorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
    FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
    InTabManager->UnregisterAllTabSpawners();
}

void FSNSequencerEditorToolkit::InitToolMenuContext(FToolMenuContext& MenuContext)
{
    FAssetEditorToolkit::InitToolMenuContext(MenuContext);
}

TSharedRef<FSNAssetPreviewScene> FSNSequencerEditorToolkit::MakePreviewScene()
{
    if (PreviewScene.IsValid())
        return PreviewScene.ToSharedRef();

    PreviewScene = MakeShared<FSNAssetPreviewScene>
    (
        FPreviewScene::ConstructionValues()
        .AllowAudioPlayback(true)
        .ShouldSimulatePhysics(true)
        // .ForceUseMovementComponentInNonGameWorld(true)

        , PreviewManager->PreviewContext
    );
    return PreviewScene.ToSharedRef();
}

bool FSNSequencerEditorToolkit::ShouldHaveMainCamera() const
{
    if (!Target)
    {
        return false;
    }

    // 存在动画相关节点才需要生成预览角色
    for (const auto& Track : Target->Tracks)
    {
        if (!Track)
            continue;

        if (!Track->IsA(USNCameraTrack::StaticClass()))
        {
            continue;
        }

        for (const auto& Node : Track->Nodes)
        {
            if (!Node)
                continue;

            if (!Node->IsA(USNCameraNode::StaticClass()))
            {
                continue;
            }

            return true;
        }
    }

    return false;
}

void FSNSequencerEditorToolkit::UpdatePreviewResource() const
{
    if (!ViewportWidget)
        return;

    const auto& CurrentPreviewScene = ViewportWidget->GetPreviewScene();

    if (ShouldHaveMainCamera())
    {
        if (!PreviewCamera->PreviewCamera)
        {
            const auto& Actor = PreviewCamera->SpawnCamera(CurrentPreviewScene.Get(), ASNPreviewActor::StaticClass());

            PreviewCamera->PreviewCamera = Actor;
            UE_LOG(LogScenarioNarrativeEditor, Display, TEXT("Main actor created."));
        }
    }
    else
    {
        if (PreviewCamera->PreviewCamera)
        {
            PreviewCamera->PreviewCamera = nullptr;

            UE_LOG(LogScenarioNarrativeEditor, Display, TEXT("Main actor removed."));
        }
    }

    // 存在动画相关节点才需要生成预览角色
    for (const auto& Track : Target->Tracks)
    {
        if (!Track)
            continue;

        if (!Track->IsA(USNCameraTrack::StaticClass()))
        {
            continue;
        }

        for (const auto& Node : Track->Nodes)
        {
            if (!Node)
                continue;

            if (Node->IsA(USNSplineCameraNode::StaticClass()))
            {
                if (!Context->PreviewManager->PreviewContext->GetResource<FSNAssetPreviewSpline>()->GetSplineForNode(Cast<USNSplineCameraNode>(Node.Get())))
                {
                    Context->PreviewManager->PreviewContext->GetResource<FSNAssetPreviewSpline>()->AcquireSplineForNode(Cast<USNSplineCameraNode>(Node.Get()));
                }
            }
        }
    }
}

// 视口中点击Actor
void FSNSequencerEditorToolkit::OnElementSelectionChanged(const UTypedElementSelectionSet* SelectionSet)
{
    InspectorWidget->SetObject(ViewportWidget->GetSNViewportClient()->SNEE->GetSelectedActor());
}

// 大纲中点击Actor
void FSNSequencerEditorToolkit::OnActorPicked(AActor* PickedActor)
{
    InspectorWidget->SetObject(PickedActor);
}

void FSNSequencerEditorToolkit::OnNodeCreated(TObjectPtr<USNNodeBase> InNode) const
{
    if (!PreviewCamera.IsValid())
    {
        UpdatePreviewResource();
    }
}

void FSNSequencerEditorToolkit::OnNodeDeleted(TObjectPtr<USNNodeBase> InNode) const
{
    if (!PreviewCamera.IsValid())
    {
        UpdatePreviewResource();
    }
}

void FSNSequencerEditorToolkit::OnTrackDeleted(TObjectPtr<USNTrackBase> InTrack) const
{
    if (!PreviewCamera.IsValid())
    {
        UpdatePreviewResource();
    }
}

void FSNSequencerEditorToolkit::ExportAssetToJson()
{
    if (!Context)
        return;

    Context->ExportSequenceToJson();
}

// void FSNSequencerEditorToolkit::OnTransSNChanged(const FTransSNContext& TransSNContext, ETransSNStateEventType State)
// {
//     UE_LOG(LogScenarioNarrativeEditor, Display, TEXT("on transSN %s changed, state %d"), *TransSNContext.Title.ToString(), State);
// }

void FSNSequencerEditorToolkit::OnUndo(const FTransactionContext& TransSNContext, bool Succeeded)
{
    UE_LOG(LogScenarioNarrativeEditor, Display, TEXT("on %s undo"), *TransSNContext.Title.ToString());
}

void FSNSequencerEditorToolkit::OnRedo(const FTransactionContext& TransSNContext, bool Succeeded)
{
    UE_LOG(LogScenarioNarrativeEditor, Display, TEXT("on %s redo"), *TransSNContext.Title.ToString());
}

void FSNSequencerEditorToolkit::NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent,
                                                     FProperty* PropertyThatChanged)
{
    const FName PropertyName = PropertyChangedEvent.GetPropertyName();

    if (PropertyName == GET_MEMBER_NAME_CHECKED(USNCameraNode, KeyFrames))
    {
        int event_num = PropertyChangedEvent.GetNumObjectsBeingEdited();
        for(int i = 0; i < event_num; i++)
        {
            if(const USNCameraNode* CameraNode = Cast<USNCameraNode>(PropertyChangedEvent.GetObjectBeingEdited(i)))
            {
                Context->NodeToWidget[CameraNode]->Repaint();
            }
            else if (const USNSplineCameraNode* SplineCameraNode = Cast<USNSplineCameraNode>(PropertyChangedEvent.GetObjectBeingEdited(i)))
            {
                Context->NodeToWidget[SplineCameraNode]->Repaint();
            }
        }
    }
    else if(PropertyName == GET_MEMBER_NAME_CHECKED(FSNCameraKeyFrame, Time))
    {
        const TArray<TWeakObjectPtr<UObject>>& SelectedObjects = InspectorWidget->GetSelectedObjects();

        // 遍历弱引用数组
        for (const TWeakObjectPtr<UObject>& WeakObjPtr : SelectedObjects)
        {
            // 正确方式：直接使用Get()获取裸指针并检查有效性
            if (UObject* Object = WeakObjPtr.Get())
            {
                // 确保对象没有被垃圾回收
                if (IsValid(Object))
                {
                    USNCameraKeyFrameWrapper* SNCameraKeyFrameWrapper = Cast<USNCameraKeyFrameWrapper>(Object);
                    SNCameraKeyFrameWrapper->SourceNode->KeyFrames[SNCameraKeyFrameWrapper->KeyFrameId] = SNCameraKeyFrameWrapper->CameraKeyFrame;
                }
            }
        }
    }
    if (PropertyName == GET_MEMBER_NAME_CHECKED(USNPerformerNode, KeyFrames))
    {
        int event_num = PropertyChangedEvent.GetNumObjectsBeingEdited();
        for(int i = 0; i < event_num; i++)
        {
            if(const USNPerformerNode* Node = Cast<USNPerformerNode>(PropertyChangedEvent.GetObjectBeingEdited(i)))
            {
                Context->NodeToWidget[Node]->Repaint();
            }
        }
    }
    else if (PropertyName == GET_MEMBER_NAME_CHECKED(USNPerformerNode, Animation))
    {
        int event_num = PropertyChangedEvent.GetNumObjectsBeingEdited();
        for (int i = 0; i < event_num; i++)
        {
            if (USNPerformerNode* Node =  const_cast<USNPerformerNode*>(Cast<USNPerformerNode>(PropertyChangedEvent.GetObjectBeingEdited(i))))
            {
                if (SSNNodeWidget* Widget = *Context->NodeToWidget.Find(Node))
                {
                    Node->bActiveLoopMode = !Node->Animation->HasRootMotion();
                    Node->EndTime = Node->BeginTime + Node->Animation->GetPlayLength();
                    Widget->Invalidate(EInvalidateWidget::LayoutAndVolatility);
                }
            }
        }
    }

}

void FSNSequencerEditorToolkit::HandleObjectPendingDelete(const TArray<UObject*>& ObjectsForDelete)
{
    for (const UObject* Obj : ObjectsForDelete)
    {
        if (!Obj->IsA(USNSequencer::StaticClass()))
        {
            continue;
        }

        if (PreviewManager.IsValid())
        {

            PreviewManager->State = ESNPreviewPlayerState::Stopped;
        }

        // TODO: back some memory here
    }
}

#undef LOCTEXT_NAMESPACE
