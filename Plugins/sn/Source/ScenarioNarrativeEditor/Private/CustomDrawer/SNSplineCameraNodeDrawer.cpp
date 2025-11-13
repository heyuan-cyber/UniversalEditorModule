#include "SNSplineCameraNodeDrawer.h"

#include "SNSequencerContext.h"
#include "SCurveEditor.h"
#include "Misc/SNGeneralDragDropOp.h"
#include "Node/SNSplineCameraNode.h"
#include "Widget/SNTimeRulerWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Rendering/DrawElements.h"
#include "SequencerEditor/SNNodeWidget.h"
#include "Styling/SlateTypes.h"
#include "Styling/SlateBrush.h"

void FSNSplineCameraNodeDrawer::OnSlateRepaint(const USNAssetBase* Asset, const TSharedPtr<SOverlay>& Slot, const FSNSequencerContext* Context)
{
    const USNSplineCameraNode* Node = Cast<USNSplineCameraNode>(Asset);

    // 创建共享指针用于拖动操作
    TSharedPtr<SWidget> DragWidget;

    for (int32 Index = 0; Index < Node->KeyFrames.Num(); ++Index)
    {
        const int32 CurrentIndex = Index;
        Slot->AddSlot()
        [
            SNew(SBox)
            [
                SNew(SBox)
                .Padding_Lambda([Asset, Slot, CurrentIndex]
                {
                    constexpr float HalfWidth = 20;
                    const USNSplineCameraNode* Node = Cast<USNSplineCameraNode>(Asset);
                    const float Left = Slot->GetCachedGeometry().Size.X * Node->KeyFrames[CurrentIndex].Time / Node->Length();
                    const float Right = Slot->GetCachedGeometry().Size.X - Left - HalfWidth;
                    return FMargin(Left - HalfWidth, 0, Right, 0);
                })
                [
                    SNew(SOverlay)
                    + SOverlay::Slot()
                    .HAlign(HAlign_Center)
                    .VAlign(VAlign_Center)
                    [
                        SNew(SBox)
                        .WidthOverride(11.31f)
                        .HeightOverride(11.31f)
                        [
                            // 使用 SAssignNew 获取控件引用
                            SAssignNew(DragWidget, SBorder)

                            .BorderImage(FCoreStyle::Get().GetBrush("NoBorder"))
                            // 鼠标按下事件 - 开始拖动检测
                            .OnMouseButtonDown_Lambda([Context, CurrentIndex, Asset, DragWidget, this](const FGeometry& Geometry, const FPointerEvent& MouseEvent) -> FReply
                            {
                                if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
                                {
                                    USNSplineCameraNode* Node = Cast<USNSplineCameraNode>(const_cast<USNAssetBase*>(Asset));
                                    USNSplineCameraKeyFrameWrapper* Wrapper = CreateTransientObject<USNSplineCameraKeyFrameWrapper>();
                                    Wrapper->SplineCameraKeyFrame = Node->KeyFrames[CurrentIndex];
                                    Wrapper->KeyFrameId = CurrentIndex;
                                    Wrapper->SourceNode = Node;
                                    // 暂时先这样写后续看时候要去掉const
                                    const_cast<FSNSequencerContext*>(Context)->SetSelectUObject(Wrapper);
                                    // 开始检测拖动
                                    return FReply::Handled();
                                }
                                return FReply::Unhandled();
                            })
                            // 鼠标释放事件 - 点击操作
                            .OnMouseButtonUp_Lambda([CurrentIndex, Asset, Context](const FGeometry& Geometry, const FPointerEvent& MouseEvent) -> FReply
                            {
                                if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
                                {
                                    USNSplineCameraNode* Node = Cast<USNSplineCameraNode>(const_cast<USNAssetBase*>(Asset));
                                    Node->SortKeyFramesByTime();
                                    Context->NodeToWidget[Node]->Repaint();
                                    return FReply::Handled();
                                }
                                return FReply::Unhandled();
                            })
                            .OnMouseMove_Lambda([Node, CurrentIndex, Context, Asset, DragWidget](const FGeometry& Geometry, const FPointerEvent& MouseEvent) -> FReply
                            {
                                if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
                                {
                                    FSlateApplication::Get().GetPlatformCursor()->SetType(EMouseCursor::Type::ResizeLeftRight);
                                    return FReply::Handled().BeginDragDrop(FSNGeneralDragDropOp::New(FOnDraggedCallback::CreateLambda([Asset, CurrentIndex, Context](const FDragDropEvent& Evt)
                                    {
                                        auto ScaleInfo = Context->TimeRulerWidget->GetTrackScaleInfo();
                                        USNSplineCameraNode* Node = Cast<USNSplineCameraNode>(const_cast<USNAssetBase*>(Asset));
                                        float BeginX = ScaleInfo.InputToLocalX(Node->BeginTime);
                                        float MouseTime = Evt.IsShiftDown() ?
                                            Context->TimeRulerWidget->PositionToFrameTime(Evt.GetScreenSpacePosition().X - BeginX) :
                                            Context->TimeRulerWidget->PositionToTime(Evt.GetScreenSpacePosition().X - BeginX);

                                        Node->SetKeyFramesTime(CurrentIndex, FMath::Clamp(MouseTime, 0, Node->Length()));

                                    }), nullptr));
                                }
                                return FReply::Unhandled();
                            })
                            [
                                SNew(SImage)
                                .Image(FAppStyle::GetBrush("Brushes.White"))
                                .ColorAndOpacity_Lambda([this, Context, CurrentIndex]() -> FSlateColor {
                                    UObject* object = const_cast<FSNSequencerContext*>(Context)->GetCurrentSelectedUObject();
                                    if (object != nullptr) // 假设 UObjectA 有一个 bool 属性 bIsActive
                                    {
                                        USNSplineCameraKeyFrameWrapper* SplineCameraKeyFrameWrapper = Cast<USNSplineCameraKeyFrameWrapper>(object);
                                        if(SplineCameraKeyFrameWrapper)
                                            return SplineCameraKeyFrameWrapper->KeyFrameId == CurrentIndex ? FLinearColor::Red : FLinearColor::White; // 激活时为绿色
                                    }
                                    return FLinearColor::White; // 未激活时为红色
                                })
                                .RenderTransformPivot(FVector2D(0.5f, 0.5f))
                                .RenderTransform(FSlateRenderTransform(FQuat2D(FMath::DegreesToRadians(45))))
                            ]
                        ]
                    ]
                ]
            ]
        ];
    }
}

int FSNSplineCameraNodeDrawer::OnWidgetPaint(const USNAssetBase* Asset, const FSNSequencerContext* Context, const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    if (!Asset->Validate())
        return LayerId;

    const USNSplineCameraNode* Node = Cast<USNSplineCameraNode>(Asset);
    const float X = AllottedGeometry.Size.X * 0 / Node->Length();
    const FSlateLayoutTransform Transform({X, 0});

    FSlateDrawElement::MakeBox
    (
        OutDrawElements,
        ++LayerId,
        AllottedGeometry.ToPaintGeometry({X, AllottedGeometry.Size.Y}, {}),
        FAppStyle::Get().GetBrush("Brushes.White"),
        ESlateDrawEffect::None,
        FColor(127, 255, 255)
    );

    return LayerId;
}
