#pragma once

namespace ScenarioNarrativeEditor
{
    class FUtils;
}

DECLARE_LOG_CATEGORY_EXTERN(LogScenarioNarrativeEditor, Log, All);

using Utils = ScenarioNarrativeEditor::FUtils;
using FSNClipDataPtr = TWeakObjectPtr<class USNAnimationClipData>;

DECLARE_DELEGATE(FOnOperateDone);

DECLARE_DELEGATE_OneParam(FOnAnimationChanged, UAnimSequenceBase*)
DECLARE_DELEGATE_OneParam(FOnSelctedClipChanged, FSNClipDataPtr);
