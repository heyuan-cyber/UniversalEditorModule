#include "PreviewTask/SNScenePreviewTask.h"
#include "Node/SNSceneNode.h"
#include "PreviewScene/SNAssetPreviewScene.h"
#include "PreviewScene/Viewport/SNAssetViewport.h"
#include "Track/SNSceneTrack.h"
#include "Preview/SNPreviewContext.h"


void FSNScenePreviewTask::Enter(const FSNPreviewContext* Context)
{
    TObjectPtr<USNSceneNode> Node = Cast<USNSceneNode>(Context->Node);
    if (!Node) return;
    TObjectPtr<USNSceneTrack> Track = Cast<USNSceneTrack>(Node->Parent);
    if (!Track) return;
    TSharedPtr<FSNAssetPreviewScene> Manager = Context->Viewport.Pin()->GetPreviewScene();
    if (!Manager) return;

    Manager->SpawnLevel(Node->LevelClass);
}

void FSNScenePreviewTask::Sample(const FSNPreviewManager* Context, float CurrentTime, float PrevTime)
{

}

void FSNScenePreviewTask::Exit(const FSNPreviewContext* Context)
{
    TObjectPtr<USNSceneNode> Node = Cast<USNSceneNode>(Context->Node);
    if (!Node) return;
    TObjectPtr<USNSceneTrack> Track = Cast<USNSceneTrack>(Node->Parent);
    if (!Track) return;
    TSharedPtr<FSNAssetPreviewScene> Manager = Context->Viewport.Pin()->GetPreviewScene();
    if (!Manager) return;

    Manager->UnspawnLevel();
}
