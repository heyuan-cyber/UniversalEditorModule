#include "Preview/UI/SNCameraPreview.h"

#include "Camera/CameraComponent.h"
#include "EditorEngine/SNEditorEngine.h"
#include "Widgets/SViewport.h"



FCameraPreviewClient::FCameraPreviewClient(UWorld* World, const TSharedPtr<SEditorViewport>& InEditorViewport)
    : FEditorViewportClient(nullptr, nullptr, StaticCastSharedPtr<SEditorViewport>(InEditorViewport))
{
    EditorWorld = World;
}

FCameraPreviewClient::~FCameraPreviewClient()
{

}

UWorld* FCameraPreviewClient::GetWorld() const
{
    return EditorWorld;
}

void FCameraPreviewClient::Tick(float DeltaSeconds)
{
    FEditorViewportClient::Tick(DeltaSeconds);

    UpdateViewForLockedActor(DeltaSeconds);

    Viewport->Invalidate();   // 请求重绘
}

void FCameraPreviewClient::UpdateViewForLockedActor(float DeltaSeconds)
{
    bUseControllingActorViewInfo = false;
    ControllingActorViewInfo = FMinimalViewInfo();
    ControllingActorAspectRatioAxisConstraint.Reset();
    ControllingActorExtraPostProcessBlends.Empty();
    ControllingActorExtraPostProcessBlendWeights.Empty();

    AActor* Actor = USNEditorEngine::Get()->GetSelectedActor();
    if (Actor != NULL)
    {
        // Check if the viewport is transitioning
        FViewportCameraTransform& ViewTransform = GetViewTransform();
        if (!ViewTransform.IsPlaying())
        {
            // Update transform
            if (Actor->GetAttachParentActor() != NULL)
            {
                // Actor is parented, so use the actor to world matrix for translation and rotation information.
                SetViewLocation(Actor->GetActorLocation());
                SetViewRotation(Actor->GetActorRotation());
            }
            else if (Actor->GetRootComponent() != NULL)
            {
                // No attachment, so just use the relative location, so that we don't need to
                // convert from a quaternion, which loses winding information.
                SetViewLocation(Actor->GetRootComponent()->GetRelativeLocation());
                SetViewRotation(Actor->GetRootComponent()->GetRelativeRotation());
            }

            if (true)
            {
                UCameraComponent* CamComp = Cast<UCameraComponent>(Actor->GetComponentByClass(UCameraComponent::StaticClass()));

                if (CamComp != nullptr)
                {
                    if (ensure(CamComp->GetEditorPreviewInfo(DeltaSeconds, ControllingActorViewInfo)))
                    {
                        bUseControllingActorViewInfo = true;
                        CamComp->GetExtraPostProcessBlends(ControllingActorExtraPostProcessBlends,
                                                           ControllingActorExtraPostProcessBlendWeights);

                        // Post processing is handled by OverridePostProcessingSettings
                        ViewFOV = ControllingActorViewInfo.FOV;
                        AspectRatio = ControllingActorViewInfo.AspectRatio;
                        SetViewLocation(ControllingActorViewInfo.Location);
                        SetViewRotation(ControllingActorViewInfo.Rotation);
                    }
                }
            }

            const double DistanceToCurrentLookAt = FVector::Dist(GetViewLocation(), GetLookAtLocation());

            const FQuat CameraOrientation = FQuat::MakeFromEuler(GetViewRotation().Euler());
            FVector Direction = CameraOrientation.RotateVector(FVector(1, 0, 0));

            SetLookAtLocation(GetViewLocation() + Direction * DistanceToCurrentLookAt);
        }
    }
}

const float SCameraPreview::PreviewTextPadding = 3.0f;

void SCameraPreview::Construct(const FArguments& InArgs)
{
    const int32 HorizSpacingBetweenViewports = 18;
    const int32 PaddingBeforeBorder = 6;

    TSharedPtr<SViewport> PreviewViewport;
    auto GetPreviewContent = [&PreviewViewport](const FArguments& InOpArgs)-> TSharedRef<SWidget>
    {
        if (InOpArgs._Content.Widget == SNullWidget::NullWidget)
        {
            return SAssignNew(PreviewViewport, SViewport)
                .RenderDirectlyToWindow(false)
                .IsEnabled(FSlateApplication::Get().GetNormalExecutionAttribute())
                .EnableGammaCorrection(false) // Scene rendering handles gamma correction
                .EnableBlending(true);
        }
        else
        {
            return InOpArgs._Content.Widget;
        }
    };
    // We usually don't want actor preview viewports to be interactive at all, but some custom actor previews may want to override this
    EVisibility BorderVisibility = EVisibility::HitTestInvisible;

    //We draw certain buttons depending on whether we're in editor or VR mode
    EVisibility VRVisibility = EVisibility::Hidden;
    EVisibility EditorVisibility = EVisibility::Visible;
    this->ChildSlot
    [
        SNew(SBorder)
    .Padding(0)

    .Visibility(EVisibility::SelfHitTestInvisible)

    .BorderImage(FAppStyle::GetBrush("NoBorder"))
    .HAlign(HAlign_Right)
    .VAlign(VAlign_Bottom)
    .Padding(FMargin(0, 0, PaddingBeforeBorder, PaddingBeforeBorder))
        [
            SNew(SOverlay)
            + SOverlay::Slot()
            [
                SNew(SBorder)
                .Visibility(BorderVisibility)
                .Padding(16.0f)
                .BorderImage(FAppStyle::GetBrush("UniformShadow_Tint"))
                .BorderBackgroundColor(this, &SCameraPreview::GetBorderColorAndOpacity)
                .ColorAndOpacity(this, &SCameraPreview::GetColorAndOpacity)

                [
                    SNew(SBox)
                        .WidthOverride(this, &SCameraPreview::OnReadWidth)
                        .HeightOverride(this, &SCameraPreview::OnReadHeight)
                    [
                        SNew(SOverlay)
                        + SOverlay::Slot()
                        [
                            GetPreviewContent(InArgs)
                        ]

                        + SOverlay::Slot()
                          .Padding(PreviewTextPadding)
                          .HAlign(HAlign_Center)
                        [
                            SNew(STextBlock)
                                        .Text(this, &SCameraPreview::OnReadText)
                                        .Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
                                        .ShadowOffset(FVector2D::UnitVector)
                                        .WrapTextAt(this, &SCameraPreview::OnReadTextWidth)
                        ]
                        + SOverlay::Slot()
                          .Padding(PreviewTextPadding)
                          .HAlign(HAlign_Center)
                          .VAlign(VAlign_Bottom)
                        [
                            SNew(STextBlock)
                                    .Text(this, &SCameraPreview::OnFilmbackText)
                                    .Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
                                    .ShadowOffset(FVector2D::UnitVector)
                        ]
                    ]
                ]

            ]
            + SOverlay::Slot()
              .HAlign(HAlign_Left)
              .VAlign(VAlign_Bottom)
              .Padding(24.0f)
            [
                // Create a button to pin/unpin this viewport
                SNew(SButton)
                .ContentPadding(0)
                .ForegroundColor(FSlateColor::UseForeground())
                .ButtonStyle(FAppStyle::Get(), "ToggleButton")

                .IsFocusable(false)
                             [
                                 SNew(SImage)
                        .Visibility(EVisibility::Visible)
                        .Image(this, &SCameraPreview::GetPinButtonIconBrush)
                             ]

                // Bind the button's "on clicked" event to our object's method for this
                .OnClicked(this, &SCameraPreview::OnTogglePinnedButtonClicked)
                .Visibility(EditorVisibility)

                // Pass along the block's tool-tip string
                .ToolTipText(this, &SCameraPreview::GetPinButtonToolTipText)
            ]
            + SOverlay::Slot()
              .HAlign(HAlign_Left)
              .VAlign(VAlign_Bottom)
              .Padding(0)
            [
                SNew(SBox)
            .WidthOverride(45)
            .HeightOverride(45)
                [
                    // Create a button to pin/unpin this viewport
                    SNew(SButton)
                    .ContentPadding(0)
                    .ForegroundColor(FSlateColor::UseForeground())
                    .ButtonStyle(FAppStyle::Get(), "ToggleButton")

                    .IsFocusable(false)
                                 [
                                     SNew(SImage)
                            .Visibility(EVisibility::Visible)
                            .Image(this, &SCameraPreview::GetVRPinButtonIconBrush)
                                 ]

                    // Bind the button's "on clicked" event to our object's method for this
                    .OnClicked(this, &SCameraPreview::OnTogglePinnedButtonClicked)
                    .Visibility(VRVisibility)

                    // Pass along the block's tool-tip string
                    .ToolTipText(this, &SCameraPreview::GetPinButtonToolTipText)
                ]
            ]
            + SOverlay::Slot()
              .HAlign(HAlign_Right)
              .VAlign(VAlign_Bottom)
              .Padding(0)
            [
                SNew(SBox)
            .WidthOverride(45)
            .HeightOverride(45)
                [
                    // Create a button to attach/detach this viewport
                    SNew(SButton)
                    .ContentPadding(0)
                    .ForegroundColor(FSlateColor::UseForeground())
                    .ButtonStyle(FAppStyle::Get(), "ToggleButton")
                    .IsFocusable(false)
                                 [
                                     SNew(SImage)
                            .Visibility(EVisibility::Visible)
                            .Image(this, &SCameraPreview::GetDetachButtonIconBrush)
                                 ]

                    // Bind the button's "on clicked" event to our object's method for this
                    .OnClicked(this, &SCameraPreview::OnToggleDetachButtonClicked)
                    .Visibility(VRVisibility)

                    // Pass along the block's tool-tip string
                    .ToolTipText(this, &SCameraPreview::GetDetachButtonToolTipText)
                ]
            ]
        ]
    ];

    ViewportWidget = PreviewViewport;
    PreviewActorPtr = InArgs._PreviewActor;
}

FReply SCameraPreview::OnTogglePinnedButtonClicked()
{
    return FReply::Handled();
}

const FSlateBrush* SCameraPreview::GetVRPinButtonIconBrush() const
{
    const FSlateBrush* IconBrush = nullptr;

    if (true)
    {
        IconBrush = FAppStyle::GetBrush("VRViewportActorPreview.Pinned");
    }
    else
    {
        IconBrush = FAppStyle::GetBrush("VRViewportActorPreview.Unpinned");
    }

    return IconBrush;
}

const FSlateBrush* SCameraPreview::GetPinButtonIconBrush() const
{
    const FSlateBrush* IconBrush = nullptr;

    if (true)
    {
        IconBrush = FAppStyle::GetBrush("ViewportActorPreview.Pinned");
    }
    else
    {
        IconBrush = FAppStyle::GetBrush("ViewportActorPreview.Unpinned");
    }

    return IconBrush;
}

FText SCameraPreview::GetPinButtonToolTipText() const
{
    FTextKey LLOCTEXT_NAMESPACE;
    FText CurrentToolTipText = LOCTEXT("PinPreviewActorTooltip", "Pin Preview");

    return CurrentToolTipText;
}

FReply SCameraPreview::OnToggleDetachButtonClicked()
{
    return FReply::Handled();
}

const FSlateBrush* SCameraPreview::GetDetachButtonIconBrush() const
{
    const FSlateBrush* IconBrush = nullptr;
    if (true)
    {
        IconBrush = FAppStyle::GetBrush("VRViewportActorPreview.Attached");
    }
    else
    {
        IconBrush = FAppStyle::GetBrush("VRViewportActorPreview.Detached");
    }
    return IconBrush;
}

FText SCameraPreview::GetDetachButtonToolTipText() const
{
    FTextKey LLOCTEXT_NAMESPACE;
    FText CurrentToolTipText = LOCTEXT("DetachPreviewActorTooltip", "Detach Preview from actor");

    return CurrentToolTipText;
}

FSlateColor SCameraPreview::GetBorderColorAndOpacity() const
{
    FLinearColor Color(0.f, 0.f, 0.f, 0.5f);

    return Color;
}

FText SCameraPreview::OnReadText() const
{
    if( PreviewActorPtr.IsValid() )
    {
        return FText::FromString(PreviewActorPtr.Get()->GetActorLabel());
    }
    else
    {
        return FText::GetEmpty();
    }
}

FText SCameraPreview::OnFilmbackText() const
{
    if (PreviewActorPtr.IsValid())
    {
        UCameraComponent* CameraComponent = Cast<UCameraComponent>(PreviewActorPtr.Get()->GetComponentByClass(UCameraComponent::StaticClass()));
        if (CameraComponent)
        {
            return CameraComponent->GetFilmbackText();
        }
    }
    return FText::GetEmpty();
}

FOptionalSize SCameraPreview::OnReadWidth() const
{
    const float PreviewHeight = OnReadHeight().Get();

    //return PreviewHeight * 1.7777f;
    return 400;
}

FOptionalSize SCameraPreview::OnReadHeight() const
{
    const float MinimumHeight = 32;
    // Also used as parent height in case valid parent viewport is not set
    const float MaximumHeight = 428;
    // Used to make sure default viewport scale * parent viewport height = roughly same size as original windows
    const float PreviewScalingFactor = 0.06308f;

    float ParentHeight = MaximumHeight;

    //return FMath::Clamp( GetDefault<ULevelEditorViewportSettings>()->CameraPreviewSize * ParentHeight * PreviewScalingFactor, MinimumHeight, MaximumHeight );
    return 250;
}

float SCameraPreview::OnReadTextWidth() const
{
    return OnReadWidth().Get() - (PreviewTextPadding*2.0f);
}
