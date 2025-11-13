// SNTrackNodes.h
// @author : xiangyp
// @date : 2025/04/23 14:48:50
// @desc : 轨道绘制节点的那部分

#pragma once
#include "CoreMinimal.h"

class USNNodeBase;
class FSNSequencerContext;
class SSNNodeWidget;
class SSNTimeRulerWidget;
class USNTrackBase;

class SSNTrackNodes : public SCompoundWidget
{
    SLATE_BEGIN_ARGS(SSNTrackNodes) {}
        SLATE_ARGUMENT(TSharedPtr<SSNTimeRulerWidget>, TimeRuler)
        SLATE_ARGUMENT(TObjectPtr<USNTrackBase>, TrackData)
        SLATE_ARGUMENT(TSharedPtr<FSNSequencerContext>, Context)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);
    virtual ~SSNTrackNodes() override;

    static USNNodeBase* CachedHoverNode;

    virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;
    virtual FReply OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

    virtual void Repaint();
    void DeleteNode();

private:
    static FVector2f CachedMousePoint;

    TSharedPtr<SSNTimeRulerWidget> TimeRuler;
    TSharedPtr<SOverlay> Slot;

    TObjectPtr<USNTrackBase> Data;
    TSharedPtr<FSNSequencerContext> Context;
    // TArray<TSharedPtr<SSNNodeWidget>> NodeWidgets;

    TSharedRef<SWidget> OnCreteContextMenu();
    void AddNode(UClass* Class);
    void AddBlueprintNodeMenu(FMenuBuilder& MenuBuilder);
};
