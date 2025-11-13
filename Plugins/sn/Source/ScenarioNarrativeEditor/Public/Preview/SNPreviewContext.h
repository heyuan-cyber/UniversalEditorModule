#pragma once
#include "Node/SNPerformerNode.h"
#include "Track/SNTrackBase.h"
#include "Preview/SNAssetPreviewCamera.h"
#include "Preview/UI/SNAssetPreviewHUD.h"

class USNSequencer;
class USNAssetBase;
class FSNAssetPreviewScene;
class FSNAssetPreviewSpline;
class FSNAssetPreviewPerformer;
class SSNAssetViewport;
class FSNAssetViewportClient;

struct SCENARIONARRATIVEEDITOR_API FSNPreviewContext
{
    TWeakPtr<SSNAssetViewport> Viewport;
    TSharedPtr<FSNAssetPreviewCamera> Camera;
    TSharedPtr<FSNAssetPreviewHUD> HUD;
    TSharedPtr<FSNAssetPreviewPerformer> Performer;
    TObjectPtr<USNSequencer> AssetSequence;
    TObjectPtr<AActor> Actor;
    TObjectPtr<UWorld> World;
    TObjectPtr<USNNodeBase> Node;
    TObjectPtr<USNAssetBase> Track;

    FSNAssetPreviewScene* PreviewScene = nullptr;
    FSNAssetPreviewSpline* PreviewSpline = nullptr;

    template<typename T>
    T* GetResource() const;

    template<typename T>
    TSharedPtr<T> GetSharedResource() const;

    template<typename T>
    TWeakPtr<T> GetWeakResource() const;

    template<typename T>
    TObjectPtr<T> GetObjectResource() const;

    template<typename T>
    void SetResource(T* NewResource);

    template<typename T>
    void SetResource(const TSharedPtr<T>& NewResource);

    template<typename T>
    void SetResource(const TWeakPtr<T>& NewResource);

    template<typename T>
    void SetResource(const TObjectPtr<T>& NewResource);
};
