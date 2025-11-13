#include "SNTypeRegistryHelper.h"

#include "ScenarioNarrativeEditorDefines.h"
#include "ContentBrowserMenuContexts.h"
#include "EditorModeRegistry.h"
#include "WholeSNNodes.h"
#include "CustomDrawer/KeyFrame/SNKeyFrameDrawer.h"
#include "ScenarioNarrativeSequencer/SNSequencerTypeActions.h"
#include "DataStyle/SNAssetStyleRegistry.h"
#include "DataStyle/Node/SNDialogNodeStyle.h"
#include "DataStyle/Node/SNPerformerNodeStyle.h"
#include "DataStyle/Node/SNSceneNodeStyle.h"
#include "DataStyle/Node/SNSplineCameraNodeStyle.h"
#include "Preview//SNAssetPreviewRegistry.h"
#include "DataStyle/Track/SNDialogTrackStyle.h"
#include "DataStyle/Track/SNPerformerTrackStyle.h"
#include "DataStyle/Track/SNSceneTrackStyle.h"
#include "Interfaces/IPluginManager.h"
#include "MontageSet/SNMontageSetActionTypes.h"
#include "MontageSet/SNMontageSetEditorToolMenuContext.h"
#include "Node/SNDialogNode.h"
#include "Node/SNPerformerNode.h"
#include "PreviewTask/SNHUDPreviewTask.h"
#include "Styling/SlateStyleRegistry.h"
#include "Task/SNPreviewTaskRegistry.h"
#include "Track/SNDialogTrack.h"
#include "Track/SNSceneTrack.h"
#include "Node/SNSceneNode.h"
#include "Node/SNSplineCameraNode.h"
#include "PreviewTask/SNScenePreviewTask.h"
#include "PreviewTask/SNSplineCameraNodePreviewTask.h"
#include "Track/SNPerformerTrack.h"
using namespace ScenarioNarrativeEditor;

#define LOCTEXT_NAMESPACE "FSNTypeRegistryHelper"

// #define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( StyleSet->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define IMAGE_PLUGIN_BRUSH( RelativePath, ... ) FSlateImageBrush( FSNTypeRegistryHelper::InContent( RelativePath, ".png" ), __VA_ARGS__ )

FSNTypeRegistryHelper::FSNTypeRegistryHelper() : bIsInited(false)
{
}

FSNTypeRegistryHelper::~FSNTypeRegistryHelper()
{
    UnregisterAll();
}

FSNTypeRegistryHelper& FSNTypeRegistryHelper::Get()
{
    static TSharedRef<FSNTypeRegistryHelper> GRegistryHelper = MakeShared<FSNTypeRegistryHelper>();
    return GRegistryHelper.Get();
}

const FSlateBrush* FSNTypeRegistryHelper::GetBrush(const FName PropertyName) const
{
    return StyleSet->GetBrush(PropertyName);
}

FString FSNTypeRegistryHelper::InContent(const FString& RelativePath, const ANSICHAR* Extension)
{
    static FString ContentDir = IPluginManager::Get().FindPlugin(TEXT("ScenarioNarrative"))->GetBaseDir() / TEXT("Resources/Slate");
    return (ContentDir / RelativePath) + Extension;
}

void FSNTypeRegistryHelper::RegisterStartupCallback()
{
    UToolMenus::Get()->RegisterStartupCallback
    (
        FSimpleMulticastDelegate::FDelegate::CreateStatic(CreateMontageSetViaAnimationSequence)
    );
}

void FSNTypeRegistryHelper::Initialize()
{
    if (bIsInited)
    {
        return;
    }

    RegisterSlateStyleSet();
    FSlateStyleRegistry::RegisterSlateStyle(*StyleSet);

    RegisterAssetTypeActions();
    auto& AssetToolsModule = FAssetToolsModule::GetModule().Get();
    for (const auto& TypeSN : TypeSNs)
    {
        AssetToolsModule.RegisterAssetTypeActions(TypeSN);
    }

    RegisterNodeTasks();
    RegisterDataStyle();
    RegisterDrawer();

    bIsInited = true;
    UE_LOG(LogScenarioNarrativeEditor, Log, TEXT("SN Type Registry Initialized"));
}

void FSNTypeRegistryHelper::UnregisterAll()
{
    if (!bIsInited)
    {
        return;
    }

    // unregister slate style set
    FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet);

    // unregister type SNs
    if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
    {
        auto& AssetToolsModule = FAssetToolsModule::GetModule().Get();
        for (const auto& TypeSN : TypeSNs)
        {
            AssetToolsModule.UnregisterAssetTypeActions(TypeSN);
        }
    }

    // unregister node tasks
    FSNPreviewTaskRegistry::Get().Clear();

    // unregister data styles
    FSNAssetStyleRegistry::Get().Clear();

    // unregister custom drawer
    FSNCustomActionDrawerRegistry::Get().Clear();
}

void FSNTypeRegistryHelper::RegisterSlateStyleSet()
{
    StyleSet = MakeShared<FSlateStyleSet>("ScenarioNarrativeEditorStyleSet");
    StyleSet->SetContentRoot(FPaths::ProjectPluginsDir() / TEXT("ScenarioNarrative/Resources"));

    StyleSet->Set("Icons.PhysicsTrack", new IMAGE_PLUGIN_BRUSH("Physics64", CoreStyleConstants::Icon64x64));
    StyleSet->Set("Icons.SNTrack", new IMAGE_PLUGIN_BRUSH("SN64", CoreStyleConstants::Icon64x64));
    StyleSet->Set("Icons.InputTrack", new IMAGE_PLUGIN_BRUSH("Input64", CoreStyleConstants::Icon64x64));

    StyleSet->Set("ClassThumbnail.MontageSet", new IMAGE_PLUGIN_BRUSH("MontageSetIcon64", CoreStyleConstants::Icon64x64));
    StyleSet->Set("ClassThumbnail.SNSequencer", new IMAGE_PLUGIN_BRUSH("SNSequencerIcon64", CoreStyleConstants::Icon64x64));
    // register slate style set here...
}

void FSNTypeRegistryHelper::RegisterDataStyle()
{
    auto& Registry = FSNAssetStyleRegistry::Get();
    Registry.Register(USNAnimationNode::StaticClass(), new FSNAnimationNodeStyle);
    Registry.Register(USNMontageSetNode::StaticClass(), new FSNMontageSetNodeStyle);
    Registry.Register(USNAnimationTrack::StaticClass(), new FSNAnimationTrackStyle);
    Registry.Register(USNPhysicsTrack::StaticClass(), new FSNPhysicsTrackStyle);
    Registry.Register(USNPhysicsCastNode::StaticClass(), new FSNPhysicsCastNodeStyle);
    Registry.Register(USNVfxTrack::StaticClass(), new FSNVfxTrackStyle);
    Registry.Register(USNNiagaraParticleNode::StaticClass(), new FSNNiagaraParticleNodeStyle);
    Registry.Register(USNInputTrack::StaticClass(), new FSNInputTrackStyle);
    Registry.Register(USNBehaviorTrack::StaticClass(), new FSNBehaviorTrackStyle);
    Registry.Register(USNEnableInputNode::StaticClass(), new FSNEnableInputNodeStyle);
    Registry.Register(USNRecordInputNode::StaticClass(), new FSNRecordInputNodeStyle);
    Registry.Register(USNTranslationNode::StaticClass(), new FSNTranslationNodeStyle);
    Registry.Register(USNEditorDebugTrack::StaticClass(), new FSNEditorDebugTrackStyle);
    Registry.Register(USNCameraTrack::StaticClass(), new FSNCameraTrackStyle);
    Registry.Register(USNCameraNode::StaticClass(), new FSNCameraNodeStyle);
    Registry.Register(USNSplineCameraNode::StaticClass(), new FSNSplineCameraNodeStyle);
    Registry.Register(USNDialogNode::StaticClass(), new FSNDialogNodeStyle);
    Registry.Register(USNDialogTrack::StaticClass(), new FSNDialogTrackStyle);
    Registry.Register(USNSceneNode::StaticClass(), new FSNSceneNodeStyle);
    Registry.Register(USNSceneTrack::StaticClass(), new FSNSceneTrackStyle);
    // register data styles here...
}

void FSNTypeRegistryHelper::RegisterDrawer()
{
    auto& Registry = FSNCustomActionDrawerRegistry::Get();
    Registry.Register(USNAnimationNode::StaticClass(), new FSNAnimationNodeDrawer);
    Registry.Register(USNMontageSetNode::StaticClass(), new FSNMontageSetNodeDrawer);
    Registry.Register(USNRecordInputNode::StaticClass(), new FSNRecordInputNodeDrawer);
    Registry.Register(USNCameraNode::StaticClass(), new FSNCameraNodeDrawer);
    Registry.Register(USNSplineCameraNode::StaticClass(), new FSNSplineCameraNodeDrawer);
    Registry.Register(USNPerformerNode::StaticClass(), new FSNKeyFrameDrawer);
    // register custom drawer here...
}

void FSNTypeRegistryHelper::RegisterAssetTypeActions()
{
    TypeSNs.Add(MakeShared<FSNSequencerTypeActions>());
    TypeSNs.Add(MakeShared<FSNMontageSetTypeActions>());
    // more type SNs here...
}

void FSNTypeRegistryHelper::RegisterNodeTasks()
{
    auto& Registry = FSNPreviewTaskRegistry::Get();
    Registry.Register<USNAnimationNode>(new FSNAnimationNodePreviewTask);
    Registry.Register<USNMontageSetNode>(new FSNMontageSetNodePreviewTask);
    Registry.Register<USNPhysicsCastNode>(new FSNPhysicsCastNodePreviewTask);
    Registry.Register<USNNiagaraParticleNode>(new FSNNiagaraParticleNodePreviewTask);
    Registry.Register<USNCameraNode>(new FSNCameraNodePreviewTask);
    Registry.Register<USNDialogNode>(new FSNHUDPreviewTask);
    Registry.Register<USNSceneNode>(new FSNScenePreviewTask);
    Registry.Register<USNSplineCameraNode>(new FSNSplineCameraNodePreviewTask);
    // more node tasks here...
}

void FSNTypeRegistryHelper::RegisterNodePreviewers()
{
    auto& Registry = FSNAssetPreviewRegistry::Get();

    Registry.Register(USNCameraNode::StaticClass(), new FSNAssetPreviewCamera());
}

void FSNTypeRegistryHelper::CreateMontageSetViaAnimationSequence()
{
    FToolMenuOwnerScoped OwnerScoped(UE_MODULE_NAME);
    UToolMenu* AnimSequenceContextMenu = UE::ContentBrowser::ExtendToolMenu_AssetContextMenu(
        UAnimSequence::StaticClass());

    FToolMenuSection& Section = AnimSequenceContextMenu->AddSection
    (
        UE_MODULE_NAME,
        LOCTEXT("SNSectionName", "ScenarioNarrative"),
        FToolMenuInsert("GetAssetSNs", EToolMenuInsertType::Before)
    );

    Section.AddDynamicEntry(NAME_None, FNewToolMenuSectionDelegate::CreateLambda([](FToolMenuSection& InSection)
    {
        FToolUIAction UISN(FToolMenuExecuteAction::CreateLambda([](const FToolMenuContext& InContext)
        {
            const UContentBrowserAssetContextMenuContext* Context =
                UContentBrowserAssetContextMenuContext::FindContextWithAssets(InContext);
            TArray<UObject*> Assets = Context->LoadSelectedObjects<UObject>();

            TArray<UAnimSequence*> Animations;
            for (UObject* Obj : Assets)
            {
                if (UAnimSequence* AnimObject = Cast<UAnimSequence>(Obj))
                {
                    Animations.Add(AnimObject);
                }
            }

            USNMontageSetEditorToolMenuContext::OnCreateMontageSetClicked(Animations);
        }));

        InSection.AddMenuEntry
        (
            "AnimSequenceBase_CreateMontageSet",
            FText::FromString("Create Montage Set"),
            FText::FromString(TEXT("从动画中创建动作切片信息")),
            FSlateIcon(FAppStyle::GetAppStyleSetName(), "ClassIcon.AnimMontage"),
            UISN
        );
    }));
}

#undef LOCTEXT_NAMESPACE
