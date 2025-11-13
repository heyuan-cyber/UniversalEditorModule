#include "CustomDrawer\KeyFrame\SNKeyFrameDrawer.h"

#include "ScenarioNarrativeFunctionLibrary.h"
#include "SCurveEditor.h"
#include "SNSequencerContext.h"
#include "DataStyle/SNAssetStyleRegistry.h"
#include "DataStyle/Node/KeyFrame/SNKeyFrameNodeStyle.h"
#include "Misc/SNGeneralDragDropOp.h"
#include "Node/SNCameraNode.h"
#include "Node/SNNodeBase.h"
#include "Node/KeyFrame/SNKeyFrame.h"
#include "Widgets/SCompoundWidget.h"
#include "Rendering/DrawElements.h"
#include "SequencerEditor/SNNodeWidget.h"
#include "Styling/SlateTypes.h"
#include "Styling/SlateBrush.h"
#include "Widget/SNTimeRulerWidget.h"

void FSNKeyFrameDrawer::OnSlateRepaint(const USNAssetBase* Asset, const TSharedPtr<SOverlay>& Slot, const FSNSequencerContext* Context)
{
    if (!Asset || !Asset->GetClass()->ImplementsInterface(USNKeyFrameInterface::StaticClass()))
    {
        return; // Asset 不支持关键帧接口
    }

    // 获取接口
    ISNKeyFrameInterface* KeyFrameProvider = Cast<ISNKeyFrameInterface>(const_cast<USNAssetBase*>(Asset));
    if (!KeyFrameProvider)
    {
        return;
    }

    TArray<FSNKeyFrame>& KeyFrames = KeyFrameProvider->GetKeyFrames();
    USNNodeBase* Node = const_cast<USNNodeBase*>(Cast<USNNodeBase>(Asset));

    // 创建共享指针用于拖动操作
    TSharedPtr<SWidget> DragWidget;

    for (int32 Index = 0; Index < KeyFrames.Num(); ++Index)
    {
        const int32 CurrentIndex = Index;
        Slot->AddSlot()
            [
                SNew(SBox)
                    [
                        SNew(SBox)
                            .Padding_Lambda([&KeyFrames, Node, Slot, CurrentIndex]
                                {
                                    constexpr float HalfWidth = 20;
                                    const float Left = Slot->GetCachedGeometry().Size.X * KeyFrames[CurrentIndex].LocalTime / Node->Length();
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
                                                    .OnMouseButtonDown_Lambda([DragWidget, KeyFrameProvider, Node, &KeyFrames, Context, CurrentIndex, Asset, this](const FGeometry& Geometry, const FPointerEvent& MouseEvent) -> FReply
                                                        {
                                                            if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
                                                            {
                                                                FMenuBuilder MenuBuilder(true, nullptr);

                                                                MenuBuilder.AddMenuEntry(
                                                                    FText::FromString("Delete KeyFrame"),
                                                                    FText::FromString("Delete this keyframe"),
                                                                    FSlateIcon(),
                                                                    FUIAction(FExecuteAction::CreateLambda([Node, CurrentIndex, KeyFrameProvider, Context]()
                                                                        {
                                                                            KeyFrameProvider->RemoveKeyFrame(CurrentIndex);
                                                                            Context->NodeToWidget[Node]->Repaint();
                                                                        }))
                                                                );

                                                                FSlateApplication::Get().PushMenu(
                                                                    DragWidget.ToSharedRef(),
                                                                    FWidgetPath(),
                                                                    MenuBuilder.MakeWidget(),
                                                                    MouseEvent.GetScreenSpacePosition(),
                                                                    FPopupTransitionEffect(FPopupTransitionEffect::ContextMenu)
                                                                );

                                                                return FReply::Handled();
                                                            }
                                                            if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
                                                            {
                                                                USNKeyFrameWrapper* Wrapper = FScenarioNarrativeFunctionLibrary::CreateTransientObject<USNKeyFrameWrapper>();
                                                                Wrapper->KeyFrame = KeyFrames[CurrentIndex];
                                                                Wrapper->KeyFrameId = CurrentIndex;
                                                                Wrapper->SourceNode = Node;
                                                                // 暂时先这样写后续看时候要去掉const
                                                                const_cast<FSNSequencerContext*>(Context)->SetSelectUObject(Wrapper);
                                                                // 开始检测拖动
                                                                return FReply::Handled();
                                                            }
                                                            return FReply::Unhandled();
                                                        })
                                                    // 鼠标释放事件 - 点击操作 TODO: 该事件被OnMouseMove_Lambda吞了
                                                    .OnMouseButtonUp_Lambda([KeyFrameProvider, Node, CurrentIndex, Asset, Context](const FGeometry& Geometry, const FPointerEvent& MouseEvent) -> FReply
                                                        {
                                                            if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
                                                            {
                                                                KeyFrameProvider->SortKeyFrames();
                                                                Context->NodeToWidget[Node]->Repaint();
                                                                return FReply::Handled();
                                                            }
                                                            return FReply::Unhandled();
                                                        })
                                                    .OnMouseMove_Lambda([KeyFrameProvider, Node, CurrentIndex, Context](const FGeometry& Geometry, const FPointerEvent& MouseEvent) -> FReply
                                                        {
                                                            if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
                                                            {
                                                                FSlateApplication::Get().GetPlatformCursor()->SetType(EMouseCursor::Type::ResizeLeftRight);
                                                                return FReply::Handled().BeginDragDrop(FSNGeneralDragDropOp::New(
                                                                    FOnDraggedCallback::CreateLambda([KeyFrameProvider, Node, CurrentIndex, Context](const FDragDropEvent& Evt)
                                                                        {
                                                                            auto ScaleInfo = Context->TimeRulerWidget->GetTrackScaleInfo();
                                                                            float BeginX = ScaleInfo.InputToLocalX(Node->BeginTime);
                                                                            float MouseTime = Evt.IsShiftDown() ?
                                                                                Context->TimeRulerWidget->PositionToFrameTime(Evt.GetScreenSpacePosition().X - BeginX) :
                                                                                Context->TimeRulerWidget->PositionToTime(Evt.GetScreenSpacePosition().X - BeginX);
                                                                            KeyFrameProvider->SetKeyFramesTime(CurrentIndex, FMath::Clamp(MouseTime, 0, Node->Length()));
                                                                        }),
                                                                    FOnDropCallback::CreateLambda([KeyFrameProvider, Node, Context](bool bHandled, const FPointerEvent& Evt)
                                                                        {
                                                                            // 拖拽结束时的处理逻辑
                                                                            UObject* object = const_cast<FSNSequencerContext*>(Context)->GetCurrentSelectedUObject();
                                                                            if (object != nullptr && IsValid(object))
                                                                            {
                                                                                USNKeyFrameWrapper* KeyFrameWrapper = Cast<USNKeyFrameWrapper>(object);
                                                                                if (KeyFrameWrapper && Node == KeyFrameWrapper->SourceNode)
                                                                                {
                                                                                    FSNKeyFrameNodeStyle* Style = static_cast<FSNKeyFrameNodeStyle*>(FSNAssetStyleRegistry::Get().GetNodeStyle(Node->GetClass()));
                                                                                    Style->OnKeyFrameClicked(KeyFrameWrapper);
                                                                                }
                                                                            }
                                                                            KeyFrameProvider->SortKeyFrames();
                                                                            Context->NodeToWidget[Node]->Repaint();
                                                                        })
                                                                ));
                                                            }
                                                            return FReply::Unhandled();
                                                        })

                                                    [
                                                        SNew(SImage)
                                                            .Image(FAppStyle::GetBrush("Brushes.White"))
                                                            .ColorAndOpacity_Lambda([Node, this, Context, CurrentIndex]() -> FSlateColor {
                                                            UObject* object = const_cast<FSNSequencerContext*>(Context)->GetCurrentSelectedUObject();
                                                            //if (object != nullptr && !object->IsPendingKillEnabled())
                                                            if (object != nullptr)
                                                            {
                                                                USNKeyFrameWrapper* KeyFrameWrapper = Cast<USNKeyFrameWrapper>(object);
                                                                if (KeyFrameWrapper && Node == KeyFrameWrapper->SourceNode)
                                                                    return KeyFrameWrapper->KeyFrameId == CurrentIndex ? FLinearColor::Red : FLinearColor::White;
                                                            }
                                                            return FLinearColor::White;
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

int FSNKeyFrameDrawer::OnWidgetPaint(const USNAssetBase* Asset, const FSNSequencerContext* Context, const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    if (!Asset->Validate())
        return LayerId;

    const USNNodeBase* Node = Cast<USNNodeBase>(Asset);
    const float X = AllottedGeometry.Size.X * 0 / Node->Length();
    const FSlateLayoutTransform Transform({ X, 0 });

    FSlateDrawElement::MakeBox
    (
        OutDrawElements,
        ++LayerId,
        AllottedGeometry.ToPaintGeometry({ X, AllottedGeometry.Size.Y }, {}),
        FAppStyle::Get().GetBrush("Brushes.White"),
        ESlateDrawEffect::None,
        FColor(127, 255, 255)
    );

    return LayerId;
}
