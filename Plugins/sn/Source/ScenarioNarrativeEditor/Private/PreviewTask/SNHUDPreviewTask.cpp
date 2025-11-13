#include "PreviewTask/SNHUDPreviewTask.h"
#include "Node/SNDialogNode.h"
#include "Track/SNDialogTrack.h"
#include "Preview/SNPreviewContext.h"
#include "Preview/UI/SNAssetPreviewHUD.h"

void FSNHUDPreviewTask::Enter(const FSNPreviewContext* Context)
{
    TObjectPtr<USNDialogNode> Node = Cast<USNDialogNode>(Context->Node);
    if (!Node) return;
    TObjectPtr<USNDialogTrack> Track = Cast<USNDialogTrack>(Node->Parent);
    if (!Track) return;
    TSharedPtr<FSNAssetPreviewHUD> HUDManager = Context->HUD;
    if (!HUDManager) return;

    TObjectPtr<UUserWidget> HUD = NULL;
    if (!HUDManager->HUDs.Num() || !HUDManager->HUDs.Find(Track))
    {
        if(!Track->HUDClass) return;
        HUD = HUDManager->SpawnWidget(Track->HUDClass);
        HUDManager->HUDs.Add({ Track, HUD });
    }
    else
    {
        HUD = HUDManager->HUDs[Track];
    }
    HUDManager->UpdateWidget(HUD, Cast<USNDialogNode>(Node)->NodeData);
    HUDManager->ShowWidget(HUD);
}

void FSNHUDPreviewTask::Sample(const FSNPreviewManager* Context, float CurrentTime, float PrevTime)
{

}

void FSNHUDPreviewTask::Exit(const FSNPreviewContext* Context)
{
    TObjectPtr<USNDialogNode> Node = Cast<USNDialogNode>(Context->Node);
    if (!Node) return;
    TObjectPtr<USNDialogTrack> Track = Cast<USNDialogTrack>(Node->Parent);
    if (!Track) return;
    TSharedPtr<FSNAssetPreviewHUD> HUDManager = Context->HUD;
    if (!HUDManager) return;

    if (HUDManager->HUDs.Find(Node->Parent))
    {
        HUDManager->HideWidget(HUDManager->HUDs[Node->Parent]);
    }
}
