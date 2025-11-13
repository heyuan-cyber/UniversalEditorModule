#include "SNTimeRulerWidget.h"
#include "SScrubWidget.h"
#include "Fonts/FontMeasure.h"
#include "EditorEngine/Setting/SNEditorSetting.h"

#pragma optimize("", off)
#define LOCTEXT_NAMESPACE "TimeRuler"

void SSNTimeRulerWidget::Construct(const FArguments& InArgs)
{
    bShowScrubHandle = InArgs._bShowScrubHandle;
    ValueAttribute = InArgs._Value;
    OnValueChanged = InArgs._OnValueChanged;
    DistanceDragged = 0.0f;
    SequenceLength = InArgs._SequenceLength;
    ViewInputMin = InArgs._ViewInputMin;
    ViewInputMax = InArgs._ViewInputMax;
    OnSetInputViewRange = InArgs._OnSetInputViewRange;
    OnMajorTickCreated = InArgs._OnMajorTickCreated;

    bMouseMovedDuringPanning = false;
    bDragging = false;
    bPanning = false;

    bAllowZoom = InArgs._bAllowZoom;

    UpdateFrameRate();
}

FVector2D SSNTimeRulerWidget::ComputeDesiredSize(float X) const
{
    return FVector2D(100, 30);
}

void SSNTimeRulerWidget::UpdateFrameRate()
{
    USNEditorSetting* Settings = GetMutableDefault<USNEditorSetting>();
    NumOfKey = FMath::CeilToInt(Settings->FrameRate * SequenceLength.Get()); // 总帧数
    ONE_FRAME_TIME = 1.f / Settings->FrameRate;
}

int32 SSNTimeRulerWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
                                int32 LayerId,
                                const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    const int32 BackgroundLayer = LayerId;

    const FSlateFontInfo SmallLayoutFont = FCoreStyle::GetDefaultFontStyle("Regular", 10);

    const bool bEnabled = ShouldBeEnabled(bParentEnabled);
    const ESlateDrawEffect DrawEffects = bEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;

    const int32 TextLayer = BackgroundLayer + 1;

    const FSlateBrush* StyleInfo = FAppStyle::Get().GetBrush("Brushes.Recessed");
    const double GeomHeight = AllottedGeometry.GetLocalSize().Y;

    const FTrackScaleInfo TimeScaleInfo(ViewInputMin.Get(), ViewInputMax.Get(), 0.f, 0.f, AllottedGeometry.GetLocalSize());

    if (GetNumOfKey() > 0 && SequenceLength.Get() > 0)
    {
        const int32 Divider = SScrubWidget::GetDivider(ViewInputMin.Get(), ViewInputMax.Get(), AllottedGeometry.GetLocalSize(), SequenceLength.Get(),
                                                       GetNumOfKey());
        const float HalfDivider = Divider / 2.f;

        TArray<float> MajorTickPosX;
        for (float KeyVal = 0; KeyVal < GetNumOfKey(); KeyVal += HalfDivider)
        {
            const float CurValue = KeyVal * GetONE_FRAME_TIME();
            const float XPos = TimeScaleInfo.InputToLocalX(CurValue);

            if (FGenericPlatformMath::Fmod(KeyVal, Divider) == 0.f)
            {
                const FVector2D Offset(XPos, 0.f);
                const FVector2D Size(1, GeomHeight);
                // draw each box with key frame
                FSlateDrawElement::MakeBox(
                    OutDrawElements,
                    BackgroundLayer,
                    AllottedGeometry.ToPaintGeometry(Size, FSlateLayoutTransform(Offset)),
                    StyleInfo,
                    DrawEffects,
                    InWidgetStyle.GetColorAndOpacityTint()
                );

                const int32 FrameNumber = static_cast<int32>(KeyVal);
                const FString FrameString = FString::Printf(TEXT("%d"), FrameNumber);
                const FVector2D TextOffset(XPos + 2.f, 0.f);

                const TSharedRef<FSlateFontMeasure> FontMeasureService = FSlateApplication::Get().GetRenderer()->
                                                                                                  GetFontMeasureService();
                const FVector2D TextSize = FontMeasureService->Measure(FrameString, SmallLayoutFont);

                FSlateDrawElement::MakeText(
                    OutDrawElements,
                    TextLayer,
                    AllottedGeometry.ToPaintGeometry(TextSize, FSlateLayoutTransform(TextOffset)),
                    FrameString,
                    SmallLayoutFont,
                    DrawEffects);

                if (OnMajorTickCreated.IsBound())
                {
                    MajorTickPosX.Add(XPos);
                }
            }
            else if (HalfDivider > 1.f)
            {
                const float Height = static_cast<float>(GeomHeight);
                const FVector2D Offset(XPos, Height * 0.25f);
                const FVector2D Size(1, Height * 0.5f);
                // draw each box with key frame
                FSlateDrawElement::MakeBox(
                    OutDrawElements,
                    BackgroundLayer,
                    AllottedGeometry.ToPaintGeometry(Size, FSlateLayoutTransform(Offset)),
                    StyleInfo,
                    DrawEffects,
                    InWidgetStyle.GetColorAndOpacityTint()
                );
            }
        }

        if (OnMajorTickCreated.IsBound())
        {
            OnMajorTickCreated.Execute(MajorTickPosX);
        }
    }

    bool ShowScrubHandle = bShowScrubHandle.IsBound() ? bShowScrubHandle.Get() : false;
    if (SequenceLength.Get() > 0 && ShowScrubHandle)
    {
        const double XPos = TimeScaleInfo.InputToLocalX(ValueAttribute.Get());
        if (XPos >= 0)
        {
            const int32 ArrowLayer = TextLayer + 1;
            const double Height = AllottedGeometry.GetLocalSize().Y;
            const FVector2D Offset(XPos - Height * 0.25f, 0.f);
            const FSlateBrush* Brush = FAppStyle::GetBrush(TEXT("Sequencer.Timeline.VanillaScrubHandleDown"));

            FPaintGeometry MyGeometry = AllottedGeometry.ToPaintGeometry(FVector2f(Height * 0.5f, Height), FSlateLayoutTransform(Offset));
            FSlateDrawElement::MakeBox
            (
                OutDrawElements,
                ArrowLayer,
                MyGeometry,
                Brush,
                DrawEffects,
                InWidgetStyle.GetColorAndOpacityTint()
            );
        }
    }

    return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
                                    bEnabled);
}

FReply SSNTimeRulerWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    bool bHandleLeftMouseButton = MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && IsHovered();
    bool bHandleRightMouseButton = (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton || MouseEvent.GetEffectingButton() ==
            EKeys::MiddleMouseButton)
        && bAllowZoom;

    bMouseMovedDuringPanning = false;
    if (bHandleLeftMouseButton)
    {
        DistanceDragged = 0;

        // This has prevent throttling on so that viewports continue to run whilst dragging the slider
        return FReply::Handled().CaptureMouse(SharedThis(this)).PreventThrottling();
    }

    if (bHandleRightMouseButton)
    {
        bPanning = true;

        // Always capture mouse if we left or right click on the widget
        return FReply::Handled().CaptureMouse(SharedThis(this));
    }

    return FReply::Unhandled();
}

FReply SSNTimeRulerWidget::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    bool bHandleLeftMouseButton = MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && this->HasMouseCapture();
    bool bHandleRightMouseButton = (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton || MouseEvent.GetEffectingButton() ==
            EKeys::MiddleMouseButton)
        && bAllowZoom;

    if (bHandleRightMouseButton)
    {
        bPanning = false;

        FTrackScaleInfo TimeScaleInfo(ViewInputMin.Get(), ViewInputMax.Get(), 0.f, 0.f, MyGeometry.GetLocalSize());
        float CursorPosX = MyGeometry.AbsoluteToLocal(MouseEvent.GetLastScreenSpacePosition()).X;
        float NewValue = TimeScaleInfo.LocalXToInput(CursorPosX);

        if (!bMouseMovedDuringPanning)
        {
            CreateContextMenu(NewValue, MouseEvent);
        }
        return FReply::Handled().ReleaseMouseCapture();
    }

    if (bHandleLeftMouseButton)
    {
        if (!bDragging)
        {
            FTrackScaleInfo TimeScaleInfo(ViewInputMin.Get(), ViewInputMax.Get(), 0.f, 0.f, MyGeometry.GetLocalSize());
            float CursorPosX = MyGeometry.AbsoluteToLocal(MouseEvent.GetLastScreenSpacePosition()).X;
            float NewValue = TimeScaleInfo.LocalXToInput(CursorPosX);

            CommitValue(NewValue);
        }
        bDragging = false;
        return FReply::Handled().ReleaseMouseCapture();
    }

    return FReply::Unhandled();
}

FReply SSNTimeRulerWidget::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    if (!this->HasMouseCapture())
    {
        return FReply::Unhandled();
    }

    if ((MouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) || MouseEvent.IsMouseButtonDown(EKeys::MiddleMouseButton)) && bPanning)
    {
        FTrackScaleInfo ScaleInfo(ViewInputMin.Get(), ViewInputMax.Get(), 0.f, 0.f, MyGeometry.GetLocalSize());
        FVector2D ScreenDelta = MouseEvent.GetCursorDelta();
        float InputDeltaX = static_cast<float>(ScreenDelta.X / ScaleInfo.PixelsPerInput);

        bMouseMovedDuringPanning |= !ScreenDelta.IsNearlyZero(0.001f);

        float NewViewInputMin = ViewInputMin.Get() - InputDeltaX;
        float NewViewInputMax = ViewInputMax.Get() - InputDeltaX;
        // we'd like to keep  the range if outside when panning
        if (NewViewInputMin < 0.f)
        {
            NewViewInputMin = 0.f;
            NewViewInputMax = ScaleInfo.ViewInputRange;
        }
        else if (NewViewInputMax > SequenceLength.Get())
        {
            NewViewInputMax = SequenceLength.Get();
            NewViewInputMin = NewViewInputMax - ScaleInfo.ViewInputRange;
        }

        OnSetInputViewRange.ExecuteIfBound(NewViewInputMin, NewViewInputMax);
    }
    else if (!bDragging)
    {
        float CursorDeltaX = FMath::Abs(MouseEvent.GetCursorDelta().X);
        DistanceDragged += CursorDeltaX;
        if (DistanceDragged > FSlateApplication::Get().GetDragTriggerDistance())
        {
            bDragging = true;
        }
    }
    else if (bDragging)
    {
        FTrackScaleInfo TimeScaleInfo(ViewInputMin.Get(), ViewInputMax.Get(), 0.f, 0.f, MyGeometry.GetLocalSize());
        float CursorPosX = MyGeometry.AbsoluteToLocal(MouseEvent.GetLastScreenSpacePosition()).X;
        float NewValue = TimeScaleInfo.LocalXToInput(CursorPosX);

        CommitValue(NewValue);
    }

    return FReply::Handled();
}

FReply SSNTimeRulerWidget::OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    if (bAllowZoom && OnSetInputViewRange.IsBound())
    {
        FVector2D LocalMousePosition = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
        float LeftRate = LocalMousePosition.X / MyGeometry.Size.X;

        const float ZoomDelta = -0.1f * MouseEvent.GetWheelDelta();
        const float InputViewSize = ViewInputMax.Get() - ViewInputMin.Get();
        const float InputChange = InputViewSize * ZoomDelta;

        float ViewMinInput = FMath::Max(0, ViewInputMin.Get() - (InputChange * LeftRate));
        float ViewMaxInput = ViewInputMax.Get() + (InputChange * (1.f - LeftRate));

        OnSetInputViewRange.Execute(ViewMinInput, ViewMaxInput);
        return FReply::Handled();
    }

    return FReply::Unhandled();
}

FCursorReply SSNTimeRulerWidget::OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const
{
    return FCursorReply::Unhandled();
}

// absolute position
uint32 SSNTimeRulerWidget::PositionToFrame(float InPositionX) const
{
    const auto& Geometry = GetCachedGeometry();
    const FTrackScaleInfo ScaleInfo(ViewInputMin.Get(), ViewInputMax.Get(), 0, 0, Geometry.GetLocalSize());
    const FVector2D LocalPos = Geometry.AbsoluteToLocal({InPositionX, 0});

    const float InputX = ScaleInfo.LocalXToInput(LocalPos.X);
    if (InputX < 0)
    {
        return 0;
    }

    return static_cast<uint32>(InputX / GetONE_FRAME_TIME());
}

// return local position
float SSNTimeRulerWidget::FrameToPosition(uint32 InFrame) const
{
    const auto& Geometry = GetCachedGeometry();
    const FTrackScaleInfo ScaleInfo(ViewInputMin.Get(), ViewInputMax.Get(), 0, 0, Geometry.GetLocalSize());
    return ScaleInfo.InputToLocalX(InFrame * GetONE_FRAME_TIME());
}

float SSNTimeRulerWidget::FrameToTime(uint32 InFrame) const
{
    return InFrame * GetONE_FRAME_TIME();
}

uint32 SSNTimeRulerWidget::TimeToFrame(float InTime) const
{
    if (InTime < 0)
    {
        return 0;
    }

    return static_cast<uint32>(InTime / GetONE_FRAME_TIME());
}

float SSNTimeRulerWidget::PositionToTime(float InPositionX) const
{
    const auto& Geometry = GetCachedGeometry();
    const FTrackScaleInfo ScaleInfo(ViewInputMin.Get(), ViewInputMax.Get(), 0, 0, Geometry.GetLocalSize());
    const FVector2D LocalPos = Geometry.AbsoluteToLocal({InPositionX, 0});
    return FMath::Max(0, ScaleInfo.LocalXToInput(LocalPos.X));
}

float SSNTimeRulerWidget::PositionToFrameTime(float InPositionX) const
{
    return FMath::Max(0, FrameToTime(PositionToFrame(InPositionX)));
}

float SSNTimeRulerWidget::TimeToScaledLength(float InTime) const
{
    return GetTrackScaleInfo().PixelsPerInput * InTime;
}

FTrackScaleInfo SSNTimeRulerWidget::GetTrackScaleInfo() const
{
    return FTrackScaleInfo(ViewInputMin.Get(), ViewInputMax.Get(), 0, 0, GetCachedGeometry().GetLocalSize());
}

void SSNTimeRulerWidget::CreateContextMenu(float CurrentFrameTime, const FPointerEvent& MouseEvent)
{
    OnCreateContextMenu.ExecuteIfBound(CurrentFrameTime);
}

void SSNTimeRulerWidget::CommitValue(float NewValue)
{
    if (!ValueAttribute.IsBound())
    {
        ValueAttribute.Set(NewValue);
    }

    OnValueChanged.ExecuteIfBound(NewValue);
}

#pragma optimize("", on)
#undef LOCTEXT_NAMESPACE
