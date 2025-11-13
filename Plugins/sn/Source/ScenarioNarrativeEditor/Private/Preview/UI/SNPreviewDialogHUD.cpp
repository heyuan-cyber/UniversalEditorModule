#include "Preview\UI\SNPreviewDialogHUD.h"
#include "DialogNarrative/Public/DNComponent.h"

void  USNPreviewDialogHUD::Update_Implementation(const FDNNodeData& DialogData)
{
    StateName = DialogData.StateName;
    StateContent = DialogData.StateContent;
    ImagePath = DialogData.ImagePath;
    bIsSelector = DialogData.bIsSelector;
    OutPinTextArr = DialogData.OutPinTextArr;
}
