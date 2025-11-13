// Remove attributes and functions from SScrubWidget

#pragma once

#include "CoreMinimal.h"

class FPaintArgs;
class FSlateRect;
class FSlateWindowElementList;
class FText;
class FWidgetStyle;
struct FGeometry;
struct FPointerEvent;
struct FTrackScaleInfo;

DECLARE_DELEGATE_OneParam(FOnReZeroAnimSequence, int32)
DECLARE_DELEGATE_OneParam(FOnCreateContextMenu, float)
DECLARE_DELEGATE_TwoParams(FOnSetInputViewRange, float, float)
DECLARE_DELEGATE_OneParam(FOnMajorTickCreated, TArray<float>)

class SCENARIONARRATIVEEDITOR_API SSNTimeRulerWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SSNTimeRulerWidget)
            : _Value(0)
              , _NumOfKeys(10)
              , _SequenceLength()
              , _bAllowZoom(false)
              , _ViewInputMin()
              , _ViewInputMax()
              , _OnSetInputViewRange()
        {
        }
        SLATE_ATTRIBUTE(bool, bShowScrubHandle)
        SLATE_ATTRIBUTE(float, Value)
        SLATE_ATTRIBUTE(uint32, NumOfKeys)
        SLATE_ATTRIBUTE(float, SequenceLength)
        SLATE_ARGUMENT(bool, bAllowZoom)
        SLATE_ATTRIBUTE(float, ViewInputMin)
        SLATE_ATTRIBUTE(float, ViewInputMax)
        SLATE_ATTRIBUTE(float, PickedTime)
        SLATE_EVENT(FOnMajorTickCreated, OnMajorTickCreated)
        SLATE_EVENT(FOnSetInputViewRange, OnSetInputViewRange)
        SLATE_EVENT(FOnFloatValueChanged, OnValueChanged)
        // SLATE_EVENT( FOnReZeroAnimSequence, OnReZeroAnimSequence )
        SLATE_EVENT(FOnCreateContextMenu, OnCreateContextMenu)
    SLATE_END_ARGS()

    /**
     * Construct the widget
     *
     * @param InArgs   A declaration from which to construct the widget
     */
    void Construct(const FArguments& InArgs);

    // SWidget interface
    virtual FVector2D ComputeDesiredSize(float) const override;
    virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
                          FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
                          bool bParentEnabled) const override;
    virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    virtual FReply OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    virtual FCursorReply OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const override;

    uint32 PositionToFrame(float InPositionX) const;
    float FrameToPosition(uint32 InFrame) const;
    float PositionToTime(float InPositionX) const;
    float PositionToFrameTime(float InPositionX) const;
    float TimeToScaledLength(float InTime) const;

    float FrameToTime(uint32 InFrame) const;
    uint32 TimeToFrame(float InTime) const;
    uint32 GetNumOfKey() const { return NumOfKey; }
    float GetONE_FRAME_TIME() const { return ONE_FRAME_TIME; }
    void UpdateFrameRate();
    FTrackScaleInfo GetTrackScaleInfo() const;

private:
    /** Function to create context menu to display anim sequence editing options */
    void CreateContextMenu(float CurrentFrameTime, const FPointerEvent& MouseEvent);

    TAttribute<bool> bShowScrubHandle;
    TAttribute<float> ValueAttribute;
    TAttribute<float> ViewInputMin;
    TAttribute<float> ViewInputMax;
    uint32 NumOfKey;
    float ONE_FRAME_TIME;
    FOnMajorTickCreated OnMajorTickCreated;
    FOnFloatValueChanged OnValueChanged;
    FOnSetInputViewRange OnSetInputViewRange;
    FOnReZeroAnimSequence OnReZeroAnimSequence;
    FOnCreateContextMenu OnCreateContextMenu;

    /** Distance Dragged **/
    float DistanceDragged;
    /** Number of Keys **/
    TAttribute<float> SequenceLength;
    /** The number of decimal places to display */
    bool bDragging;
    bool bAllowZoom;
    /** If we are currently panning the panel*/
    bool bPanning;
    /** Has the mouse moved during panning - used to determine if we should open the context menu or not */
    bool bMouseMovedDuringPanning;


public:
    void CommitValue(float NewValue);
};
