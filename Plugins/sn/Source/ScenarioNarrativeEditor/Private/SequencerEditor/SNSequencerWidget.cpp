#include "SequencerEditor/SNSequencerWidget.h"

#include "EditorWidgetsModule.h"
#include "ITransportControl.h"
#include "SNSequencerContext.h"
#include "EditorEngine/Setting/SNEditorSetting.h"
#include "PlaybackController/SequencerPlaybackController.h"
#include "PreviewScene/Viewport/SNAssetViewport.h"
#include "PreviewScene/Viewport/SNAssetViewportClient.h"
#include "SequencerEditor/SNTrackTimeline.h"
#include "SequencerEditor/SNTrackArea.h"
#include "Widget/SNTimeRulerWidget.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSNSequencerWidget::Construct(const FArguments& InArgs)
{
    Context = InArgs._Context;
    TransportControl = MakeTransportControls();

    ChildSlot
    [
        SNew(SBox)
        .VAlign(VAlign_Fill)
        [
            SNew(SVerticalBox)
                + SVerticalBox::Slot()
                .HAlign(HAlign_Center)
                .VAlign(VAlign_Center)
                .AutoHeight()
                [
                    SNew(SHorizontalBox)
                        + SHorizontalBox::Slot()
                        .AutoWidth()
                        [
                            TransportControl.ToSharedRef()
                        ]
                        // 帧率下拉框
                        + SHorizontalBox::Slot()
                        .AutoWidth()
                        .Padding(FMargin(5, 0, 0, 0))
                        [
                            SNew(SComboButton)
                                .ButtonContent()
                                [
                                    SNew(STextBlock)
                                        .Text_Lambda([this]()
                                            {
                                                return FText::FromString(GetFrameRateText(CurrentFrameRate));
                                            })
                                ]
                            .MenuContent()
                                [
                                    GenerateFrameRateMenu()
                                ]

                        ]
                ]
            + SVerticalBox::Slot()
            .AutoHeight()
            [
                SNew(SSeparator)
                .Orientation(Orient_Horizontal)
            ]
            + SVerticalBox::Slot()
            .FillHeight(1.f)
            [
                SAssignNew(TrackTimeline, SSNTrackTimeline)
                .Asset(InArgs._Asset)
                .Context(Context)
            ]
        ]
    ];

    USNEditorSetting* Settings = GetMutableDefault<USNEditorSetting>();
    CurrentFrameRate = GetFrameRateEnumFromUint32(Settings->FrameRate);
}

TSharedRef<SWidget> SSNSequencerWidget::GenerateFrameRateMenu()
{
    FMenuBuilder MenuBuilder(true, nullptr);

    auto AddFrameRateEntry = [&](ESNFrameRate Rate, const FText& Label)
        {
            MenuBuilder.AddMenuEntry(
                Label,
                FText::FromString("Select timeline frame rate"),
                FSlateIcon(),
                FUIAction(
                    FExecuteAction::CreateRaw(this, &SSNSequencerWidget::SetFrameRate, Rate),
                    FCanExecuteAction(),
                    FIsActionChecked::CreateRaw(this, &SSNSequencerWidget::IsFrameRateChecked, Rate)
                ),
                NAME_None,
                EUserInterfaceActionType::RadioButton
            );
        };

    FTextKey LLOCTEXT_NAMESPACE;
    AddFrameRateEntry(ESNFrameRate::FPS_24, LOCTEXT("FPS24", "24 fps"));
    AddFrameRateEntry(ESNFrameRate::FPS_30, LOCTEXT("FPS30", "30 fps"));
    AddFrameRateEntry(ESNFrameRate::FPS_60, LOCTEXT("FPS60", "60 fps"));
    AddFrameRateEntry(ESNFrameRate::FPS_120, LOCTEXT("FPS120", "120 fps"));

    return MenuBuilder.MakeWidget();
}

FString SSNSequencerWidget::GetFrameRateText(ESNFrameRate Rate) const
{
    switch (Rate)
    {
        case ESNFrameRate::FPS_24: return TEXT("24 fps");
        case ESNFrameRate::FPS_30: return TEXT("30 fps");
        case ESNFrameRate::FPS_60: return TEXT("60 fps");
        case ESNFrameRate::FPS_120: return TEXT("120 fps");
    }
    return TEXT("30 fps");
}

ESNFrameRate SSNSequencerWidget::GetFrameRateEnumFromUint32(uint32 FrameRate) const
{
    if (FrameRate == uint32(24))return ESNFrameRate::FPS_24;
    if (FrameRate == uint32(30))return ESNFrameRate::FPS_30;
    if (FrameRate == uint32(60))return ESNFrameRate::FPS_60;
    if (FrameRate == uint32(120))return ESNFrameRate::FPS_120;
    return ESNFrameRate::FPS_60;
}

uint32 SSNSequencerWidget::GetFrameRateUint32(ESNFrameRate Rate) const
{
    switch (Rate)
    {
        case ESNFrameRate::FPS_24: return uint32(24);
        case ESNFrameRate::FPS_30: return uint32(30);
        case ESNFrameRate::FPS_60: return uint32(60);
        case ESNFrameRate::FPS_120: return uint32(120);
    }
    return uint32(30);
}

bool SSNSequencerWidget::IsFrameRateChecked(ESNFrameRate Rate) const
{

    return CurrentFrameRate == Rate;
}

void SSNSequencerWidget::SetFrameRate(ESNFrameRate NewRate)
{
    CurrentFrameRate = NewRate;
    USNEditorSetting* Settings = GetMutableDefault<USNEditorSetting>();
    Settings->FrameRate = GetFrameRateUint32(NewRate);
    Settings->SaveConfig();
    Context->PreviewManager->UpdateTickFrameRate(1.f / GetFrameRateUint32(NewRate));

    // 刷新 Timeline
    if (TrackTimeline.IsValid())
    {
        TrackTimeline->GetTrackAreaWidget()->GetTimeRulerWidget()->UpdateFrameRate();
        TrackTimeline->Invalidate(EInvalidateWidget::LayoutAndVolatility);
    }
}

FReply SSNSequencerWidget::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
    const FKey Key = InKeyEvent.GetKey();
    // 示例：检测回车键
    if (Key == EKeys::Enter)
    {
        Context->SavePreviewSceneUObject();
    }
    // 添加空格键播放/暂停支持
    else if (Key == EKeys::SpaceBar)
    {
        TogglePlayback();
        return FReply::Handled();
    }
    // 添加左右方向键支持
    else if (Key == EKeys::Left)
    {
        StepToPreviousFrame();
        return FReply::Handled();
    }
    else if (Key == EKeys::Right)
    {
        StepToNextFrame();
        return FReply::Handled();
    }
    return FReply::Unhandled();
}END_SLATE_FUNCTION_BUILD_OPTIMIZATION

TSharedRef<ITransportControl> SSNSequencerWidget::MakeTransportControls()
{
    FEditorWidgetsModule& EditorWidgetsModule = FModuleManager::Get().LoadModuleChecked<FEditorWidgetsModule>("EditorWidgets");
    FTransportControlArgs ControlArgs;

    // 绑定按钮功能回调
    ControlArgs.OnBackwardStep = FOnClicked::CreateSP(this, &SSNSequencerWidget::StepToPreviousFrame);
    ControlArgs.OnForwardStep = FOnClicked::CreateSP(this, &SSNSequencerWidget::StepToNextFrame);
    ControlArgs.OnForwardPlay = FOnClicked::CreateSP(this, &SSNSequencerWidget::TogglePlayback);
    ControlArgs.OnForwardEnd = FOnClicked::CreateSP(this, &SSNSequencerWidget::StopPlayback);
    ControlArgs.OnToggleLooping = FOnClicked::CreateSP(this, &SSNSequencerWidget::ToggleLooping);
    ControlArgs.OnGetLooping = FOnGetLooping::CreateSP(this, &SSNSequencerWidget::IsLooping);

    ControlArgs.OnGetPlaybackMode = FOnGetPlaybackMode::CreateSP(this, &SSNSequencerWidget::GetPlaybackMode);


    // 获取当前播放状态
    ControlArgs.OnGetPlaybackMode = FOnGetPlaybackMode::CreateSP(this, &SSNSequencerWidget::GetPlaybackMode);

    return EditorWidgetsModule.CreateTransportControl(ControlArgs);
}

// 播放/暂停切换
FReply SSNSequencerWidget::TogglePlayback()
{
    if (Context->GetPlaybackController()->IsPlaying())
    {
        Context->GetPlaybackController()->Pause();
    }
    else
    {
        Context->GetPlaybackController()->Play();
    }
    return FReply::Handled();
}

// 停止播放
FReply SSNSequencerWidget::StopPlayback()
{
    Context->GetPlaybackController()->Stop();
    return FReply::Handled();
}

// 切换循环状态
FReply SSNSequencerWidget::ToggleLooping()
{
    const bool bNewLoopingState = !Context->GetPlaybackController()->IsLooping();
    Context->GetPlaybackController()->SetLooping(bNewLoopingState);
    return FReply::Handled();
}

// 切换循环状态
bool SSNSequencerWidget::IsLooping() const
{
    return Context->GetPlaybackController()->IsLooping();
}

// 跳转到下一帧
FReply SSNSequencerWidget::StepToNextFrame()
{
    Context->GetPlaybackController()->StepToNextFrame();
    return FReply::Handled();
}

// 跳转到上一帧
FReply SSNSequencerWidget::StepToPreviousFrame()
{
    Context->GetPlaybackController()->StepToPreviousFrame();
    return FReply::Handled();
}

// 获取当前播放状态
EPlaybackMode::Type SSNSequencerWidget::GetPlaybackMode() const
{
    if (Context->GetPlaybackController() && Context->GetPlaybackController()->IsPlaying())
    {
        return EPlaybackMode::PlayingForward;
    }
    return EPlaybackMode::Stopped;
}
