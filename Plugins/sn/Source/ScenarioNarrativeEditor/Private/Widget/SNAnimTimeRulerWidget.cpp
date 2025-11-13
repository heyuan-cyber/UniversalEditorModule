#include "SNAnimTimeRulerWidget.h"

#include "EditorWidgetsModule.h"
#include "ITransportControl.h"
#include "SScrubWidget.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "Animation/DebugSkelMeshComponent.h"
#include "MontageSet/SNMontageSetAsset.h"
#include "Widgets/Input/SSpinBox.h"

void SSNAnimTimeRulerWidget::Construct(const FArguments& InArgs)
{
    SetAsset(InArgs._Asset.Get());
    if (InArgs._PreviewMesh.IsBound())
    {
        if (InArgs._PreviewMesh.Get())
        {
            AnimSingleNodeInstance = InArgs._PreviewMesh.Get()->GetSingleNodeInstance();
        }
    }

    ChildSlot
    [
        SNew(SVerticalBox)
        + SVerticalBox::Slot()
        .FillHeight(0.5f)
        [
            MakeTimeRulerWidget(InArgs)
        ]
        + SVerticalBox::Slot()
        .FillHeight(0.5f)
        .HAlign(HAlign_Center)
        .VAlign(VAlign_Fill)
        [
            MakeControlWidget()
        ]
    ];
}

void SSNAnimTimeRulerWidget::SetAsset(USNMontageSetAsset* InAsset)
{
    if (Asset == InAsset)
    {
        return;
    }

    Asset = InAsset;
}

void SSNAnimTimeRulerWidget::SetClipData(ClipDataPtr InClip)
{
    if (!AnimSingleNodeInstance.IsValid() || !InClip.IsValid())
    {
        return;
    }

    UAnimSequenceBase* Seq = Cast<UAnimSequenceBase>(AnimSingleNodeInstance->CurrentAsset);

    CurrentSelected = InClip;
    AnimSingleNodeInstance->SetPosition
    (
        FMath::Clamp
        (
            AnimSingleNodeInstance->GetCurrentTime(),
            (CurrentSelected.IsValid() ? Seq->GetTimeAtFrame(CurrentSelected->BeginFrame) : 0),
            (CurrentSelected.IsValid() ? Seq->GetTimeAtFrame(CurrentSelected->EndFrame) : 0xffffff)
        ),
        false
    );
}

void SSNAnimTimeRulerWidget::SetPreviewMesh(const TObjectPtr<UDebugSkelMeshComponent>& InMesh)
{
    if (!InMesh)
    {
        return;
    }

    AnimSingleNodeInstance = InMesh->GetSingleNodeInstance();
}

uint32 SSNAnimTimeRulerWidget::GetNumOfKeys() const
{
    if (!Asset.IsValid() || !Asset->Animation)
    {
        return 0;
    }

    return Asset->Animation->GetNumberOfSampledKeys();
}

float SSNAnimTimeRulerWidget::GetSequenceLength() const
{
    if (!Asset.IsValid() || !Asset->Animation)
    {
        return 0;
    }

    return Asset->Animation->GetPlayLength();
}

TSharedRef<SBorder> SSNAnimTimeRulerWidget::MakeTimeRulerWidget(const FArguments& InArgs)
{
    return SNew(SBorder)
        .Padding(FMargin(5, 0))
        [
            SAssignNew(ScrubWidget, SScrubWidget)
            .Value_Lambda([this]
            {
                return AnimSingleNodeInstance.IsValid()
                           ? AnimSingleNodeInstance->GetCurrentTime()
                           : 0;
            })
            .OnValueChanged_Lambda([this](float NewValue)
            {
                if (!AnimSingleNodeInstance.IsValid())
                {
                    return;
                }

                AnimSingleNodeInstance->SetPlaying(false);
                float BeginTime = CurrentSelected.IsValid()
                                      ? Asset->Animation->GetTimeAtFrame(CurrentSelected->BeginFrame)
                                      : 0;
                float EndTime = CurrentSelected.IsValid()
                                    ? Asset->Animation->GetTimeAtFrame(CurrentSelected->EndFrame)
                                    : Asset->Animation->GetPlayLength();

                AnimSingleNodeInstance->SetPosition(FMath::Clamp(NewValue, BeginTime, EndTime), false);
            })
            .DraggableBars(InArgs._DraggableBars)
            .OnBarDrag(InArgs._OnBarDrag)
            .OnBarCommit(InArgs._OnBarCommit)
            .NumOfKeys(this, &SSNAnimTimeRulerWidget::GetNumOfKeys)
            .SequenceLength(this, &SSNAnimTimeRulerWidget::GetSequenceLength)
            .ViewInputMax_Lambda([this]
            {
                if (!Asset.IsValid() || !Asset->Animation)
                {
                    return 0.f;
                }

                // 策划需求，右侧时间轴应该显示最大帧数，此处的时间轴比动画长度要多一段
                const int Divider = SScrubWidget::GetDivider(
                    0, GetSequenceLength(),
                    ScrubWidget->GetCachedGeometry().GetLocalSize(),
                    GetSequenceLength(), GetNumOfKeys());
                const FAnimKeyHelper Helper(GetSequenceLength(), GetNumOfKeys());
                return Asset->Animation->GetPlayLength() + (Divider / 2.f) * Helper.TimePerKey();
            })
            .ViewInputMin(0)
            .DisplayDrag(true)
            .bAllowZoom(false)
            .bDisplayAnimScrubBarEditing(false)
            .Clipping(EWidgetClipping::ClipToBounds)
        ];
}

TSharedRef<SWidget> SSNAnimTimeRulerWidget::MakeTransportControls() const
{
    FEditorWidgetsModule& EditorWidgetsModule = FModuleManager::Get().LoadModuleChecked<FEditorWidgetsModule>(
        "EditorWidgets");

    FTransportControlArgs TransportControlArgs;
    TransportControlArgs.bAreButtonsFocusable = false;
    TransportControlArgs.OnForwardPlay = FOnClicked::CreateLambda([this]
    {
        if (!AnimSingleNodeInstance.IsValid())
        {
            return FReply::Handled();
        }

        if (AnimSingleNodeInstance->IsPlaying())
        {
            AnimSingleNodeInstance->SetPlaying(false);
        }
        else
        {
            float StartTime = CurrentSelected.IsValid()
                                  ? Asset->Animation->GetTimeAtFrame(CurrentSelected->BeginFrame)
                                  : 0.f;
            float EndTime = CurrentSelected.IsValid() ? Asset->Animation->GetTimeAtFrame(CurrentSelected->EndFrame) : 0;
            if (AnimSingleNodeInstance->GetCurrentTime() >= EndTime)
            {
                AnimSingleNodeInstance->SetPosition(StartTime, false);
            }

            AnimSingleNodeInstance->SetPlaying(true);
        }
        return FReply::Handled();
    });

    TransportControlArgs.OnGetPlaybackMode = FOnGetPlaybackMode::CreateLambda([this]
    {
        if (!AnimSingleNodeInstance.IsValid())
        {
            return EPlaybackMode::Stopped;
        }

        return AnimSingleNodeInstance->IsPlaying() ? EPlaybackMode::PlayingForward : EPlaybackMode::Stopped;
    });

    TransportControlArgs.OnForwardStep = FOnClicked::CreateLambda([this]
    {
        if (!AnimSingleNodeInstance.IsValid())
        {
            return FReply::Handled();
        }

        AnimSingleNodeInstance->SetPlaying(false);

        float EndTime = CurrentSelected.IsValid()
                            ? Asset->Animation->GetTimeAtFrame(CurrentSelected->EndFrame)
                            : Asset->Animation->GetPlayLength();

        if (CurrentSelected.IsValid() && AnimSingleNodeInstance->GetCurrentTime() >= EndTime)
        {
            AnimSingleNodeInstance->SetPosition(EndTime, false);
        }
        else
        {
            AnimSingleNodeInstance->StepForward();
        }

        return FReply::Handled();
    });

    TransportControlArgs.OnBackwardStep = FOnClicked::CreateLambda([this]
    {
        if (!AnimSingleNodeInstance.IsValid())
        {
            return FReply::Handled();
        }

        AnimSingleNodeInstance->SetPlaying(false);

        float BeginTime = CurrentSelected.IsValid() ? Asset->Animation->GetTimeAtFrame(CurrentSelected->BeginFrame) : 0;
        if (CurrentSelected.IsValid() && AnimSingleNodeInstance->GetCurrentTime() <= BeginTime)
        {
            AnimSingleNodeInstance->SetPosition(BeginTime, false);
        }
        else
        {
            AnimSingleNodeInstance->StepBackward();
        }

        return FReply::Handled();
    });

    TransportControlArgs.OnForwardEnd = FOnClicked::CreateLambda([this]
    {
        if (!AnimSingleNodeInstance.IsValid())
        {
            return FReply::Handled();
        }

        AnimSingleNodeInstance->SetPlaying(false);
        float EndTime = CurrentSelected.IsValid()
                            ? Asset->Animation->GetTimeAtFrame(CurrentSelected->EndFrame)
                            : Asset->Animation->GetPlayLength();
        AnimSingleNodeInstance->SetPosition(EndTime, false);

        return FReply::Handled();
    });

    TransportControlArgs.OnBackwardEnd = FOnClicked::CreateLambda([this]
    {
        if (!AnimSingleNodeInstance.IsValid())
        {
            return FReply::Handled();
        }

        AnimSingleNodeInstance->SetPlaying(false);
        float BeginTime = CurrentSelected.IsValid()
                              ? Asset->Animation->GetTimeAtFrame(CurrentSelected->BeginFrame)
                              : 0.f;
        AnimSingleNodeInstance->SetPosition(FMath::Max(0, BeginTime), false);

        return FReply::Handled();
    });

    TransportControlArgs.OnToggleLooping = FOnClicked::CreateLambda([this]
    {
        if (!AnimSingleNodeInstance.IsValid())
        {
            return FReply::Handled();
        }

        AnimSingleNodeInstance->SetLooping(!AnimSingleNodeInstance->IsLooping());
        return FReply::Handled();
    });

    TransportControlArgs.OnGetLooping = FOnGetLooping::CreateLambda([this]
    {
        if (!AnimSingleNodeInstance.IsValid())
        {
            return false;
        }

        return AnimSingleNodeInstance->IsLooping();
    });

    TransportControlArgs.OnTickPlayback = FOnTickPlayback::CreateLambda([this](double InCurrentTime, float InDeltaTime)
    {
        if (!Asset.IsValid() || !Asset->Animation || !AnimSingleNodeInstance.IsValid())
        {
            return;
        }

        const int32 EndFrame = CurrentSelected.IsValid()
                                   ? CurrentSelected->EndFrame
                                   : Asset->Animation->GetNumberOfSampledKeys() - 1;
        const float BeginTime = CurrentSelected.IsValid()
                                    ? Asset->Animation->GetTimeAtFrame(CurrentSelected->BeginFrame)
                                    : 0;
        const int32 CurFrame = Asset->Animation->GetFrameAtTime(AnimSingleNodeInstance->GetCurrentTime());

        if (CurFrame >= FMath::Clamp(EndFrame, 0, Asset->Animation->GetNumberOfSampledKeys() - 1))
        {
            AnimSingleNodeInstance->SetPosition(BeginTime, false);
            if (!AnimSingleNodeInstance->IsLooping())
            {
                const float EndTime = CurrentSelected.IsValid()
                                          ? Asset->Animation->GetTimeAtFrame(CurrentSelected->EndFrame)
                                          : Asset->Animation->GetPlayLength();

                AnimSingleNodeInstance->StopAnim();
                // 单次播放结束后停在尾帧
                AnimSingleNodeInstance->SetPosition(EndTime, false);
            }
        }

        // WTF: force replay when end frame equals total frame
        if (AnimSingleNodeInstance->IsLooping() && AnimSingleNodeInstance->GetCurrentTime() < BeginTime)
        {
            AnimSingleNodeInstance->SetPosition(BeginTime, false);
        }
    });

    return EditorWidgetsModule.CreateTransportControl(TransportControlArgs);
}

TSharedRef<SHorizontalBox> SSNAnimTimeRulerWidget::MakeControlWidget() const
{
    return SNew(SHorizontalBox)
        + SHorizontalBox::Slot()
        .AutoWidth()
        .HAlign(HAlign_Center)
        .Padding(FMargin(0, 5))
        [
            SNew(SSpinBox<float>)
            .MinDesiredWidth(80)
            .Value_Lambda([this]
            {
                if (!AnimSingleNodeInstance.IsValid())
                {
                    return 1.f;
                }

                if (CurrentSelected.IsValid())
                {
                    AnimSingleNodeInstance->SetPlayRate(CurrentSelected->PlayRate);
                }

                return AnimSingleNodeInstance->GetPlayRate();
            })
            .OnValueChanged_Lambda([this](float NewValue)
            {
                if (!AnimSingleNodeInstance.IsValid())
                {
                    return;
                }

                // dont allow play reverse
                NewValue = FMath::Max(0, NewValue);
                if (CurrentSelected.IsValid())
                {
                    CurrentSelected->PlayRate = NewValue;
                }

                AnimSingleNodeInstance->SetPlayRate(NewValue);
            })
            .MaxFractionalDigits(2)
            .ToolTipText(FText::FromString("Play Rate"))
        ]
        + SHorizontalBox::Slot()
        .AutoWidth()
        [
            MakeTransportControls()
        ]
        + SHorizontalBox::Slot()
        .Padding(FMargin(5, 0))
        .MaxWidth(200)
        .MinWidth(200)
        .VAlign(VAlign_Center)
        [
            SNew(STextBlock)
            .Text_Lambda([this]()
            {
                if (!AnimSingleNodeInstance.IsValid())
                {
                    return FText::FromString("No Animation Loaded");
                }

                auto AnimSeq = Cast<UAnimSequenceBase>(AnimSingleNodeInstance->GetCurrentAsset());
                FNumberFormattingOptions NumberFormattingOptions;
                NumberFormattingOptions.MinimumFractionalDigits = 2;
                NumberFormattingOptions.MaximumFractionalDigits = 2;

                return FText::Format
                (
                    INVTEXT("Time: {0}/{1}, Frame: {2}/{3}"),
                    FText::AsNumber(AnimSingleNodeInstance->GetCurrentTime(), &NumberFormattingOptions),
                    FText::AsNumber(AnimSeq->GetPlayLength(), &NumberFormattingOptions),
                    FText::AsNumber(AnimSeq->GetFrameAtTime(AnimSingleNodeInstance->GetCurrentTime())),
                    FText::AsNumber(AnimSeq->GetNumberOfSampledKeys() - 1)

                );
            })
        ];
}
