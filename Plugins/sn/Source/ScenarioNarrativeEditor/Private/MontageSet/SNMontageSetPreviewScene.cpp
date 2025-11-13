#include "MontageSet/SNMontageSetPreviewScene.h"
#include "AdvancedPreviewScene.h"
#include "AnimPreviewInstance.h"
#include "ScenarioNarrativeEditorDefines.h"
#include "ComponentAssetBroker.h"
#include "Utils.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "Animation/DebugSkelMeshComponent.h"
#include "MontageSet/SNMontageSetEditorToolkit.h"

FSNMontageSetPreviewScene::FSNMontageSetPreviewScene(const ConstructionValues& CVS, const TSharedRef<FSNMontageSetEditorToolkit>& EditorToolkit)
    : FAdvancedPreviewScene(CVS), Editor(EditorToolkit)
{
    AWorldSettings* WorldSettings = GetWorld()->GetWorldSettings(true);
    WorldSettings->bEnableWorldBoundsChecks = false;

    SetFloorVisibility(true);

    // TODO: init preview scene here
}

FSNMontageSetPreviewScene::~FSNMontageSetPreviewScene()
{
}

void FSNMontageSetPreviewScene::Tick(float DeltaTime)
{
    FAdvancedPreviewScene::Tick(DeltaTime);
    GetWorld()->Tick(LEVELTICK_ViewportsOnly, DeltaTime);
}

void FSNMontageSetPreviewScene::RefreshSceneViaAnimation(UAnimSequenceBase* InAnimSequence)
{
    if (!InAnimSequence)
    {
        ClearPreviewComponent();
        if (PreviewActor)
        {
            PreviewActor->Destroy();
            PreviewActor->Reset();
            PreviewActor = nullptr;
        }
        return;
    }

    if (!PreviewActor)
    {
        PreviewActor = Cast<AActor>(SpawnPreviewActor());
    }

    auto SkeletalMeshComp = Cast<UDebugSkelMeshComponent>(
        PreviewActor->GetComponentByClass(UDebugSkelMeshComponent::StaticClass()));

    auto Skeleton = InAnimSequence->GetSkeleton();
    auto Mesh = Skeleton ? Skeleton->GetAssetPreviewMesh(InAnimSequence) : nullptr;

    if (Skeleton && Mesh)
    {
        SkeletalMeshComp->SetSkeletalMesh(Mesh);
        SkeletalMeshComp->EnablePreview(true, InAnimSequence);
        SkeletalMeshComp->PreviewInstance->SetLooping(true);
        SkeletalMeshComp->SetPlayRate(1.f);

        SkeletalMeshComp->Play(true);

        ResetPreviewComponent();
        PreviewObjectComponents = Utils::CreateAttachedObjectToSkeleton(SkeletalMeshComp, Skeleton);
    }

    UE_LOG(LogScenarioNarrativeEditor, Log, TEXT("refresh preview scene for animation %s"),
           (InAnimSequence ? *InAnimSequence->GetName() : TEXT("nullptr")));
}

TObjectPtr<UDebugSkelMeshComponent> FSNMontageSetPreviewScene::GetPreviewSkeletonMesh() const
{
    if (!PreviewActor)
    {
        return nullptr;
    }

    return PreviewActor->GetComponentByClass<UDebugSkelMeshComponent>();
}

AActor* FSNMontageSetPreviewScene::SpawnPreviewActor() const
{
    AActor* Actor = GetWorld()->SpawnActor(AActor::StaticClass());
    Actor->AddComponentByClass(UDebugSkelMeshComponent::StaticClass(), false, FTransform(), false);
    return Actor;
}

void FSNMontageSetPreviewScene::ResetPreviewComponent()
{
    for (auto* PreviewObjectComponent : PreviewObjectComponents)
    {
        if (!PreviewObjectComponent)
        {
            return;
        }

        FComponentAssetBrokerage::AssignAssetToComponent(PreviewObjectComponent, nullptr);
    }
}

void FSNMontageSetPreviewScene::ClearPreviewComponent()
{
    for (auto* Comp : PreviewObjectComponents)
    {
        RemoveComponent(Comp);
        Comp->DestroyComponent();
    }
    PreviewObjectComponents.Empty();
}
