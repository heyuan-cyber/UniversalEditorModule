// SNSequencerContext.h
// @author : xiangyp
// @date : 2025/04/22 14:46:01
// @desc : USNSequencer asset's editor context

#pragma once
#include "Track/SNTrackBase.h"

// #include "CoreMinimal.h"

class FSNPreviewManager;
class FSNAssetPreviewScene;
class FSNAssetPreviewHUD;
class  FSequencerPlaybackController;
class SSNTrackArea;
class SSNTrackHierarchy;
class USNSequencer;
class USNAssetBase;
class FSNAssetPreviewCamera;
class SSNTimeRulerWidget;
class USNNodeBase;
class SSNNodeWidget;
class SSNTrackNodes;
class SSNTrackWidget;
class USNTrackBase;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTrackDeleted, TObjectPtr<USNTrackBase>)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnNodeDeleted, TObjectPtr<USNNodeBase>)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTrackCreated, TObjectPtr<USNTrackBase>)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnNodeCreated, TObjectPtr<USNNodeBase>)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSNAssetSelected, TObjectPtr<USNAssetBase>)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSNUObjectSelected, TObjectPtr<UObject>)
DECLARE_MULTICAST_DELEGATE(FOnSaveCurrentUObjectSelected)

class FSNSequencerContext : public TSharedFromThis<FSNSequencerContext>
{
    friend class FSNSequencerEditorToolkit;
public:
    // FSNSequencerContext(USNSequencer* InAsset);
    virtual ~FSNSequencerContext()  { UE_LOG(LogTemp, Warning, TEXT("im free!!!")); }

    USNNodeBase* CurrentHoveredNode;
    USNTrackBase* CurrentHoveredTrack;

    // TODO: support multi-select ( use FMarqueeRect implement multi-select )
    // TSet<TObjectPtr<USNNodeBase>> CurrentSelectedNodes;

    USNNodeBase* GetSelectNode() const { return CurrentSelectedNode; }
    USNTrackBase* GetSelectTrack() const { return CurrentSelectedTrack; }

    SSNTimeRulerWidget* TimeRulerWidget;
    TSharedPtr<FSNPreviewManager> PreviewManager;
    TSharedPtr<FUICommandList> ToolkitCommands;

    SSNTrackHierarchy* TrackHierarchyWidget;
    SSNTrackArea* TrackAreaWidget;

    TMap<TObjectPtr<USNTrackBase>, SSNTrackWidget*> TrackToWidget;
    TMap<TObjectPtr<USNTrackBase>, SSNTrackNodes*> TrackToNodesWidget;
    TMap<TObjectPtr<USNNodeBase>, SSNNodeWidget*> NodeToWidget;

    FOnNodeCreated OnNodeCreated;
    FOnNodeDeleted OnNodeDeleted;
    FSimpleMulticastDelegate OnNodeSelectChanged;

    FOnTrackCreated OnTrackCreated;
    FOnTrackDeleted OnTrackDeleted;
    FSimpleMulticastDelegate OnTrackSelectChanged;

    FOnSNAssetSelected OnSelectChanged;
    FOnSNUObjectSelected OnSelectUObjectChanged;
    FOnSaveCurrentUObjectSelected OnSaveCurrentUObjectChanged;

    void SetSelectNode(const TObjectPtr<USNNodeBase>& Node);
    void SetSelectTrack(const TObjectPtr<USNTrackBase>& Track);
    void SetSelectUObject(const TObjectPtr<UObject>& UObject);

    void MoveTrack(USNTrackBase* Track, USNTrackBase* Target, bool bIsAfter = true);

    bool IsIntersectionWithOther(const TObjectPtr<USNNodeBase>& Node) const;
    void SetTrackIsDragging(const SSNTrackWidget* Target = nullptr);
    void ClearTrackDragging();

    bool SetTimeIfNearlyEqualOther(const TObjectPtr<USNNodeBase>& Node, const float& Time, float* Value, const float Tolerance = 0.001f) const;

    UObject* GetCurrentSelectedUObject();
    void SavePreviewSceneUObject();

    TSharedPtr<FSequencerPlaybackController> GetPlaybackController() const { return PlaybackController; }
    void InitializePlaybackController();
    float GetMaxTrackTime();
    void TickPlaybackController(float DeltaTime);
    void ChangeEditorCurrentTime(float Current);

    void ExportSequenceToJson();
    TSharedPtr<FJsonObject> TimelineDataToJson();

private:
    USNNodeBase* CurrentSelectedNode;
    USNTrackBase* CurrentSelectedTrack;
    UObject* CurrentSelectedUObject;
    TSharedPtr<FSequencerPlaybackController> PlaybackController;

    USNSequencer* Asset = nullptr;
};
