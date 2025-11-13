#include "Preview\UI\SNAssetPreviewHUD.h"
#include "DNComponent.h"
#include "Blueprint/UserWidget.h"
#include "Node/SNDialogNode.h"
#include "Preview/SNPreviewManager.h"
#include "Preview/UI/SNPreviewDialogHUD.h"
#include "PreviewScene/Viewport/SNAssetViewport.h"
#include "PreviewScene/Viewport/SNAssetViewportClient.h"
#include "Preview/SNPreviewContext.h"

FSNAssetPreviewHUD::FSNAssetPreviewHUD(TWeakPtr<FSNPreviewContext> PreviewContext)
{
    this->PreviewContext = PreviewContext;
    //FSNPreviewManager::NodeToManager.Add(USNDialogNode::StaticClass(), this);
    RegistryToManager(USNDialogNode)
}

FSNAssetPreviewHUD::~FSNAssetPreviewHUD()
{

}

void FSNAssetPreviewHUD::ShowWidget(UUserWidget* HUD) const
{
    HUD->SetVisibility(ESlateVisibility::Visible);
    PreviewContext.Pin()->Viewport.Pin()->GetSNViewportClient()->SetInputMode(true);
}

void FSNAssetPreviewHUD::HideWidget(TObjectPtr<UUserWidget> HUD) const
{
    HUD->SetVisibility(ESlateVisibility::Hidden);
    PreviewContext.Pin()->Viewport.Pin()->GetSNViewportClient()->SetInputMode(false);
}

void FSNAssetPreviewHUD::UpdateWidget(UUserWidget* HUD, FDNNodeData Message) const
{
    USNPreviewDialogHUD* DialogWidget = Cast<USNPreviewDialogHUD>(HUD);
    if(DialogWidget) DialogWidget->Update(Message);
}

TObjectPtr<UUserWidget> FSNAssetPreviewHUD::SpawnWidget(UClass* HUDClass) const
{
    TObjectPtr<UUserWidget> HUD = CreateWidget<UUserWidget>(PreviewContext.Pin()->World, HUDClass);
    PreviewContext.Pin()->Viewport.Pin()->AddViewportWidget(HUD->TakeWidget());
    return HUD;
}

void FSNAssetPreviewHUD::DestroyWidget(UUserWidget* HUD) const
{
    HUD->Destruct();
}
