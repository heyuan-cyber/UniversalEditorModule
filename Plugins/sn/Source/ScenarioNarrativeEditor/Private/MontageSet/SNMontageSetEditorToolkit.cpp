#include "MontageSet/SNMontageSetEditorToolkit.h"
#include "ScenarioNarrativeEditorDefines.h"
#include "SNMontageSetEditorToolMenuContext.h"
#include "MontageSet/SNMontageSetAsset.h"
#include "MontageSet/SNMontageClipViewport.h"
#include "MontageSet/SNMontageSetClipWidget.h"
#include "MontageSet/SNMontageSetPreviewScene.h"
#include "Widget/SNAnimTimeRulerWidget.h"
#define LOCTEXT_NAMESPACE "FSNMontageSetEditorToolkit"

FSNMontageSetEditorToolkit::FSNMontageSetEditorToolkit()
{
}

FSNMontageSetEditorToolkit::~FSNMontageSetEditorToolkit()
{
    SetAsset(nullptr);
}

void FSNMontageSetEditorToolkit::Initialize(const TArray<UObject*>& InObjects,
                                          const TSharedPtr<IToolkitHost>& EditWithinLevelEditor)
{
    if (InObjects.Num() < 1)
    {
        UE_LOG(LogScenarioNarrativeEditor, Error, TEXT("Toolkit initialize failed, InObjects.Num() < 1!"));
        return;
    }

    SAssignNew(Viewport, SSNMontageClipViewport, SharedThis(this), CreatePreviewScene());

    FPropertyEditorModule& PropertyEditorModule = FModuleManager::Get().LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    FDetailsViewArgs DetailsViewArgs;
    DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;
    DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
    DetailsTab = PropertyEditorModule.CreateDetailView(DetailsViewArgs);

    SetAsset(Cast<USNMontageSetAsset>(InObjects[0]));
    InitAssetEditor(EToolkitMode::Standalone, EditWithinLevelEditor, "MontageSetEditor",
                    CreateDefaultLayout(), true, true, InObjects);
    RegenerateMenusAndToolbars();
    FocusViewport();
}

void FSNMontageSetEditorToolkit::SetAsset(USNMontageSetAsset* InAsset)
{
    if (InAsset == CurrentAsset)
    {
        return;
    }
    if (CurrentAsset && InAsset)
    {
        RemoveEditingObject(CurrentAsset);
    }
    CurrentAsset = InAsset;
    if (DetailsTab.IsValid())
    {
        DetailsTab->SetObject(CurrentAsset);
    }
    if (PreviewScene.IsValid() && CurrentAsset)
    {
        PreviewScene->RefreshSceneViaAnimation(CurrentAsset->Animation);
    }
}

void FSNMontageSetEditorToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
    FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
    const auto WorkCategory = InTabManager->AddLocalWorkspaceMenuCategory(INVTEXT("Montage Set Editor"));

    InTabManager->RegisterTabSpawner("ViewportTab", FOnSpawnTab::CreateLambda([this](const FSpawnTabArgs& Args)
                {
                    return SNew(SDockTab)
                        [
                            Viewport.ToSharedRef()
                        ];
                }))
                .SetDisplayName(INVTEXT("Viewport"))
                .SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "GraphEditor.EventGraph_16x"))
                .SetGroup(WorkCategory);

    InTabManager->RegisterTabSpawner("DetailsTab", FOnSpawnTab::CreateLambda([this](const FSpawnTabArgs& Args)
                {
                    return SNew(SDockTab)
                        [
                            DetailsTab.ToSharedRef()
                        ];
                }))
                .SetDisplayName(INVTEXT("Debug"))
                .SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details"))
                .SetGroup(WorkCategory);

    InTabManager->RegisterTabSpawner("TimeRuler", FOnSpawnTab::CreateLambda([this](const FSpawnTabArgs& Args)
                {
                    return SNew(SDockTab)
                        [
                            SAssignNew(TimeRulerControl, SSNAnimTimeRulerWidget)
                            .Asset_Raw(this, &FSNMontageSetEditorToolkit::GetAsset)
                            .PreviewMesh_Lambda([this]
                            {
                                return CreatePreviewScene()->GetPreviewSkeletonMesh();
                            })
                            .DraggableBars_Lambda([this]
                            {
                                if (!ClipSettings.Get()) return TArray<float>();
                                auto Selected = ClipSettings->GetSelectedClipData();
                                if (Selected.IsValid() && CurrentAsset)
                                {
                                    return TArray
                                    {
                                        CurrentAsset->Animation->GetTimeAtFrame(Selected->BeginFrame),
                                        CurrentAsset->Animation->GetTimeAtFrame(Selected->EndFrame)
                                    };
                                }
                                return TArray<float>();
                            })
                            .OnBarDrag_Lambda([this](int32 InIndex, float InValue)
                            {
                                if (!ClipSettings.Get()) return;
                                auto Selected = ClipSettings->GetSelectedClipData();
                                if (!Selected.IsValid())
                                {
                                    return;
                                }
                                int Frame = CurrentAsset->Animation->GetFrameAtTime(InValue);
                                if (InIndex == 0)
                                {
                                    Selected->BeginFrame = FMath::Min(Frame, Selected->EndFrame);
                                }
                                else
                                {
                                    Selected->EndFrame = FMath::Max(Frame, Selected->BeginFrame);
                                }
                            })
                        ];
                }))
                .SetDisplayName(INVTEXT("Time Control"))
                .SetGroup(WorkCategory);
    if (!TimeRulerControl.Get()) return;
    // clips settings tab
    InTabManager->RegisterTabSpawner("ClipSettings", FOnSpawnTab::CreateLambda([this](const FSpawnTabArgs& Args)
                {
                                return SNew(SDockTab)
                        [
                            SAssignNew(ClipSettings, SSNMontageSetClipWidget)
                            .Asset_Raw(this, &FSNMontageSetEditorToolkit::GetAsset)
                            .OnAnimationChanged_Lambda([this](UAnimSequenceBase* InAnimSequence)
                            {
                                PreviewScene->RefreshSceneViaAnimation(InAnimSequence);
                                TimeRulerControl->SetPreviewMesh(PreviewScene->GetPreviewSkeletonMesh());
                            })
                            .OnSelectedClipChanged_Raw(TimeRulerControl.Get(), &SSNAnimTimeRulerWidget::SetClipData)
                        ];
                }))
                .SetDisplayName(INVTEXT("Clip Settings"))
                .SetGroup(WorkCategory);
}

void FSNMontageSetEditorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
    FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
    InTabManager->UnregisterAllTabSpawners();
}

void FSNMontageSetEditorToolkit::InitToolMenuContext(FToolMenuContext& MenuContext)
{
    FAssetEditorToolkit::InitToolMenuContext(MenuContext);
    MenuContext.AddObject(NewObject<USNMontageSetEditorToolMenuContext>());
}

TSharedRef<FTabManager::FLayout> FSNMontageSetEditorToolkit::CreateDefaultLayout()
{
    return FTabManager::NewLayout("MontageSetDefaultLayout_v1.0")->AddArea
    (
        FTabManager::NewPrimaryArea()
        ->SetOrientation(Orient_Horizontal)
        ->Split
        (
            FTabManager::NewSplitter()
            ->SetOrientation(Orient_Vertical)
            ->SetSizeCoefficient(0.8f)
            ->Split
            (
                FTabManager::NewStack()
                ->SetSizeCoefficient(0.1f)
                ->AddTab("TimeRuler", ETabState::OpenedTab)
            )
            ->Split
            (
                FTabManager::NewStack()
                ->SetSizeCoefficient(0.9f)
                ->AddTab("ViewportTab", ETabState::OpenedTab)
            )
        )
        ->Split
        (
            FTabManager::NewStack()
            ->SetSizeCoefficient(0.2f)
            ->AddTab("ClipSettings", ETabState::OpenedTab)
        )
    );
}

TSharedPtr<FSNMontageSetPreviewScene> FSNMontageSetEditorToolkit::CreatePreviewScene()
{
    if (PreviewScene.IsValid())
    {
        return PreviewScene;
    }

    PreviewScene = MakeShareable
    (
        new FSNMontageSetPreviewScene(
            FPreviewScene::ConstructionValues()
            .AllowAudioPlayback(true)
            .ShouldSimulatePhysics(true)
            .ForceUseMovementComponentInNonGameWorld(true),
            StaticCastSharedRef<FSNMontageSetEditorToolkit>(AsShared())
        )
    );
    return PreviewScene;
}

void FSNMontageSetEditorToolkit::FocusViewport() const
{
    if (!Viewport.IsValid())
    {
        return;
    }
    Viewport->OnFocusViewportToSelection();
}

void FSNMontageSetEditorToolkit::OnClose()
{
    FAssetEditorToolkit::OnClose();

    SetAsset(nullptr);
    if (PreviewScene.IsValid())
    {
        PreviewScene.Reset();
    }
    if (Viewport.IsValid())
    {
        Viewport.Reset();
    }
    if (TimeRulerControl.IsValid())
    {
        TimeRulerControl.Reset();
    }
    if (ClipSettings.IsValid())
    {
        ClipSettings.Reset();
    }
}

#undef LOCTEXT_NAMESPACE
