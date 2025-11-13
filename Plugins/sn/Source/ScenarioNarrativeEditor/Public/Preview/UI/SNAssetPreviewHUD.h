// AssetPreviewPlayer.h
// @author : niucy
// @date : 2025/08/04 16:06:00
// @desc : 编辑器预览窗口HUD管理类
#pragma once
#include "Preview/SNAssetPreviewBase.h"

struct FDNNodeData;
class UUserWidget;
class UWidget;
class FSNSequencerEditorToolkit;
class USNNodeBase;
class USNTrackBase;
struct FSNHUDPreviewContext;

class FSNAssetPreviewHUD: FSNAssetPreviewBase
{
public:
    FSNAssetPreviewHUD(TWeakPtr<FSNPreviewContext>  PreviewContext);

    virtual ~FSNAssetPreviewHUD();

    TObjectPtr<UUserWidget> SpawnWidget(UClass* WidgetClass) const;

    void DestroyWidget(UUserWidget* HUD) const;

    void ShowWidget(UUserWidget* HUD) const;

    void HideWidget(TObjectPtr<UUserWidget> HUD) const;

    void UpdateWidget(UUserWidget* HUD, FDNNodeData Message) const;

public:
    TMap<TObjectPtr<USNTrackBase>, TObjectPtr<UUserWidget>> HUDs;

    TWeakPtr<FSNPreviewContext>  PreviewContext;
};
