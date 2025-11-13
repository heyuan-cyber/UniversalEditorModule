#include "SNHotkeys.h"

#include "Styling/AppStyle.h"
#include "InputCoreTypes.h"
#include "Framework/Commands/InputChord.h"
#include "Framework/Commands/UICommandInfo.h"

#define LOCTEXT_NAMESPACE "FSNHotKeys"

const FInputChord SN_Copy = { EModifierKey::Control, EKeys::C };
const FInputChord SN_Paste = { EModifierKey::Control, EKeys::V };
const FInputChord SN_Delete = { EKeys::Delete };
const FInputChord SN_Confirm = { EKeys::SpaceBar };

FSNHotkeys::FSNHotkeys()
    : TCommands<FSNHotkeys>(TEXT("ScenarioNarrativeHotkeys"), LOCTEXT("ScenarioNarrativeEditor Commands", "Extend Editor Commands"), NAME_None, FAppStyle::GetAppStyleSetName())
{

}

void FSNHotkeys::RegisterCommands()
{
    // UI_COMMAND(AddNode, "添加节点", "向轨道中添加节点", EUserInterfaceSNType::None, Confirm);
    UI_COMMAND(DeleteSNAsset, "删除", "删除对象", EUserInterfaceActionType::Button, SN_Delete);
}

#undef LOCTEXT_NAMESPACE
