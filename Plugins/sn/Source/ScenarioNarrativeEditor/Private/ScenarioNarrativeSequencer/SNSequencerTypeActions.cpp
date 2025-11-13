#include "ScenarioNarrativeSequencer/SNSequencerTypeActions.h"

#include "SNTypeRegistryHelper.h"
#include "ScenarioNarrativeSequencer/SNSequencerEditorToolkit.h"
#include "Sequencer/SNSequencer.h"

void FSNSequencerTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects,
                                                  TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
    // FAssetTypeSNs_Base::OpenAssetEditor(InObjects, EditWithinLevelEditor);

    const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

    for (auto ObjIter = InObjects.CreateConstIterator(); ObjIter; ++ObjIter)
    {
        USNSequencer* Sequencer = Cast<USNSequencer>(*ObjIter);
        if (!Sequencer)
        {
            continue;
        }

        const TSharedRef<FSNSequencerEditorToolkit> NewEditor = MakeShared<FSNSequencerEditorToolkit>();
        NewEditor->InitAssetEditor(Mode, EditWithinLevelEditor, Sequencer);
    }
}

const FSlateBrush* FSNSequencerTypeActions::GetThumbnailBrush(const FAssetData& InAssetData,
                                                                  const FName InClassName) const
{
    return FSNTypeRegistryHelper::Get().GetBrush("ClassThumbnail.SNSequencer");
}
