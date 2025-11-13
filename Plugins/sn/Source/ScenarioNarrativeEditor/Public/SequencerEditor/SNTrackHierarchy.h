#pragma once

#include "CoreMinimal.h"

class USNTrackBase;
class FSNSequencerContext;
class SSNTrackWidget;
class USNSequencer;

class SSNTrackHierarchy : public SCompoundWidget
{
    SLATE_BEGIN_ARGS(SSNTrackHierarchy) {}
        SLATE_ARGUMENT(TObjectPtr<USNSequencer>, Asset)
        SLATE_ARGUMENT(TSharedPtr<FSNSequencerContext>, Context)
        SLATE_EVENT(FOnUserScrolled, OnUserScrolled)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);
    virtual ~SSNTrackHierarchy() override;

    static USNTrackBase* CachedHoverTrack;

    virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

    virtual void Repaint();
    void SetScrollOffset(float Offset) const;

private:
    TObjectPtr<USNSequencer> Data;

    TSharedPtr<FSNSequencerContext> Context;
    TSharedPtr<SScrollBox> Layout;

    void OpenTrackCreateMenu(const FWidgetPath& InOwnerPath, const UE::Slate::FDeprecateVector2DParameter& SummonLocation);
    TSharedRef<SWidget> OnCreteContextMenu();

    void DeleteTrack();

    void BindCommands();
};
