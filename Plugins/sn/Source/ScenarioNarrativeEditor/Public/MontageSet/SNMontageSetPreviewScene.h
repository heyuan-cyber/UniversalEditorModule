#pragma once

#include "CoreMinimal.h"
#include "AdvancedPreviewScene.h"

class USNMontageSetAsset;
class FSNMontageSetEditorToolkit;

class SCENARIONARRATIVEEDITOR_API FSNMontageSetPreviewScene : public FAdvancedPreviewScene
{
public:
    FSNMontageSetPreviewScene(const ConstructionValues& CVS, const TSharedRef<FSNMontageSetEditorToolkit>& EditorToolkit);
    virtual ~FSNMontageSetPreviewScene() override;

    virtual void Tick(float DeltaTime) override;

    TSharedRef<FSNMontageSetEditorToolkit> GetEditor() const { return Editor.Pin().ToSharedRef(); }

    void RefreshSceneViaAnimation(UAnimSequenceBase* InAnimSequence);

    TObjectPtr<AActor> GetPreviewActor() const { return PreviewActor; }
    TObjectPtr<UDebugSkelMeshComponent> GetPreviewSkeletonMesh() const;

private:
    TObjectPtr<AActor> PreviewActor;
    TWeakPtr<FSNMontageSetEditorToolkit> Editor;
    TArray<USceneComponent*> PreviewObjectComponents;

    AActor* SpawnPreviewActor() const;
    void ResetPreviewComponent();
    void ClearPreviewComponent();
};
