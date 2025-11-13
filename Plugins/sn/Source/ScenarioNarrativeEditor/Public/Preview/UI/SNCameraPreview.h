#pragma once
#include "Camera/CameraActor.h"

class FCameraPreviewClient : public FEditorViewportClient
{
public:
    FCameraPreviewClient(UWorld* World, const TSharedPtr<class SEditorViewport>& InEditorViewport);

    virtual ~FCameraPreviewClient();

    UWorld* EditorWorld;

    virtual void Tick(float DeltaSeconds) override;

    virtual UWorld* GetWorld() const override;

    void UpdateViewForLockedActor(float DeltaSeconds = 0.f);
};

class SCameraPreview : public SCompoundWidget
{
    SLATE_BEGIN_ARGS(SCameraPreview) {}

    /** Actor being previewed.*/
    SLATE_ARGUMENT( TWeakObjectPtr< ACameraActor >, PreviewActor )

    /** Optional */
    SLATE_DEFAULT_SLOT( FArguments, Content )

    SLATE_END_ARGS()
public:
    void Construct( const FArguments& InArgs );
private:
    /** Viewport widget for this actor preview */
    TSharedPtr< SViewport > ViewportWidget;

    /** Actor being previewed.*/
    TWeakObjectPtr< ACameraActor > PreviewActorPtr;

    /** Padding around the preview actor name */
    static const float PreviewTextPadding;

    FReply OnTogglePinnedButtonClicked();

    const FSlateBrush* GetVRPinButtonIconBrush() const;

    const FSlateBrush* GetPinButtonIconBrush() const;

    FText GetPinButtonToolTipText() const;

    FReply OnToggleDetachButtonClicked();

    const FSlateBrush* GetDetachButtonIconBrush() const;

    FText GetDetachButtonToolTipText() const;

    FSlateColor GetBorderColorAndOpacity() const;

    FText OnReadText() const;

    FText OnFilmbackText() const;

    FOptionalSize OnReadWidth() const;

    FOptionalSize OnReadHeight() const;

    float OnReadTextWidth() const;

public:
    /** @return    Returns this actor preview's viewport widget */
    const TSharedPtr< SViewport > GetViewportWidget() const
    {
        return ViewportWidget;
    }
};
