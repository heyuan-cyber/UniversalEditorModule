// SNTrackNodes.cpp
// @author : xiangyp
// @date : 2025/04/23 14:50:09
// @desc :

#include "SequencerEditor/SNTrackNodes.h"

#include "SNSequencerContext.h"
#include "ScenarioNarrativeEditorDefines.h"
#include "SCurveEditor.h"
#include "Widget/SNTimeRulerWidget.h"
#include "Utils.h"
#include "DataStyle/SNAssetStyleRegistry.h"
#include "DataStyle/SNNodeStyle.h"
#include "DataStyle/SNTrackStyle.h"
#include "Editor/ClassViewer/Private/UnloadedBlueprintData.h"
#include "Node/SNNodeBase.h"
#include "Node/SNBlueprintActionNode.h"
#include "SequencerEditor/SNNodeWidget.h"
#include "Track/SNTrackBase.h"

#define LOCTEXT_NAMESPACE "ActonTrackNodes"

FVector2f SSNTrackNodes::CachedMousePoint = FVector2f::ZeroVector;
USNNodeBase* SSNTrackNodes::CachedHoverNode = nullptr;

void SSNTrackNodes::Construct(const FArguments& InArgs)
{
    Data = InArgs._TrackData;
    TimeRuler = InArgs._TimeRuler;
    Context = InArgs._Context;

    ChildSlot[SAssignNew(Slot, SOverlay)];
    Repaint();
}

SSNTrackNodes::~SSNTrackNodes()
{
}

void SSNTrackNodes::Repaint()
{
    Slot->ClearChildren();
    // Context->NodeToWidget.Empty();

    Slot->AddSlot()
    [
        SNew(SBorder)
        .Visibility(EVisibility::HitTestInvisible)
        .BorderBackgroundColor_Lambda([this]
        {
            if (Context->GetSelectTrack() == Data)
            {
                return FSlateColor(FLinearColor::White.CopyWithNewOpacity(0.6f));
            }
            if (Context->CurrentHoveredTrack == Data)
            {
                return FSlateColor(FLinearColor::White.CopyWithNewOpacity(0.3f));
            }

            return FSlateColor(FLinearColor::Transparent);
        })
        .BorderImage_Lambda([this]
        {
            if (Context->GetSelectTrack() == Data || Context->CurrentHoveredTrack == Data)
            {
                return FAppStyle::GetBrush("Brushes.Hover");
            }

            return FAppStyle::GetBrush("Brushes.Panel");
        })
    ];

    for (const auto& Node : Data->Nodes)
    {
        if (!Node)
            continue;

        TSharedPtr<SSNNodeWidget> NodeWidget;
        TSharedPtr<SBox> Box;

        Slot->AddSlot().HAlign(HAlign_Fill)
        [
            SAssignNew(Box, SBox)
            .Clipping(EWidgetClipping::ClipToBoundsAlways)
            .Padding_Lambda([this, &Node]
            {
                const auto& TrackScaleInfo = TimeRuler->GetTrackScaleInfo();
                const float BeginXPos = TrackScaleInfo.InputToLocalX(Node->BeginTime);
                const float EndXPos = TrackScaleInfo.InputToLocalX(Node->EndTime);
                const float Width = TrackScaleInfo.InputToLocalX(TrackScaleInfo.ViewMaxInput);
                return FMargin(BeginXPos, 0, Width - EndXPos, 0);
            })
            [
                SAssignNew(NodeWidget, SSNNodeWidget)
                .NodeData(Node)
                .Context(Context)
            ]
        ];

        Context->NodeToWidget.Add(Node, NodeWidget.Get());
    }
}

FVector2D SSNTrackNodes::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
    const FSNTrackStyle* Style = FSNAssetStyleRegistry::Get().GetTrackStyle(Data->GetClass());
    constexpr float DONT_CARE = 100;
    return {DONT_CARE, Style->GetDesiredHeight()};
}

FReply SSNTrackNodes::OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    // track area is scroll box so must with ctrl
    if (TimeRuler && MouseEvent.IsControlDown())
    {
        return TimeRuler->OnMouseWheel(TimeRuler->GetCachedGeometry(), MouseEvent);
    }

    return SCompoundWidget::OnMouseWheel(MyGeometry, MouseEvent);
}

FReply SSNTrackNodes::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    if (IsHovered() && MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
    {
        CachedMousePoint = MouseEvent.GetScreenSpacePosition();
        FWidgetPath WidgetPath = MouseEvent.GetEventPath() != nullptr ? *MouseEvent.GetEventPath() : FWidgetPath();
        FSlateApplication::Get().PushMenu
        (
            AsShared(),
            WidgetPath,
            OnCreteContextMenu(),
            MouseEvent.GetScreenSpacePosition(),
            FPopupTransitionEffect(FPopupTransitionEffect::ContextMenu)
        );

        return FReply::Handled();
    }

    if (IsHovered() && MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        Context->SetSelectTrack(Data);
        return FReply::Handled();
    }

    return SCompoundWidget::OnMouseButtonDown(MyGeometry, MouseEvent);
}

TSharedRef<SWidget> SSNTrackNodes::OnCreteContextMenu()
{
    FMenuBuilder MenuBuilder(true, Context->ToolkitCommands);

    if (!Context->CurrentHoveredNode)
    {
        auto Classes = Utils::GetUObjectDerivedClasses<USNNodeBase>();
        if (!Classes.IsEmpty())
        {
            MenuBuilder.BeginSection(FName(TEXT("AddSNNodes")), FText::FromString("Add Node"));
            // TODO: support catalog via submenu
            for (const auto& Cls : Classes)
            {
                if (const USNNodeBase* NodeInstance = Cast<USNNodeBase>(Cls->GetDefaultObject()))
                {
                    const auto Supported = NodeInstance->AttachedToTracks();
                    if (!Supported.Contains(USNTrackBase::StaticClass()) &&
                        !NodeInstance->AttachedToTracks().Contains(Data->GetClass()))
                    {
                        continue;
                    }
                }

                const auto& Style = FSNAssetStyleRegistry::Get().GetNodeStyle(Cls);
                const FString& TypeName = Style->GetTypeName().IsEmpty() ? Cls->GetName() : Style->GetTypeName();
                MenuBuilder.AddMenuEntry
                (
                    FText::FromString(TypeName),
                    Style->GetTooltipText(nullptr),
                    Style->GetIcon(),
                    FUIAction(FExecuteAction::CreateSP(this, &SSNTrackNodes::AddNode, Cls))
                );
            }

            MenuBuilder.AddSubMenu(INVTEXT("蓝图节点"), {}, FNewMenuDelegate::CreateSP(this, &SSNTrackNodes::AddBlueprintNodeMenu));

            MenuBuilder.EndSection();
        }
    }
    else
    {
        CachedHoverNode = Context->CurrentHoveredNode;
        if (auto* Style = FSNAssetStyleRegistry::Get().GetNodeStyle(CachedHoverNode->GetClass()))
        {
            Style->OnCreateContextMenu(MenuBuilder, CachedHoverNode, Context.Get());
        }
    }

    return MenuBuilder.MakeWidget();
}

void SSNTrackNodes::AddNode(UClass* Class)
{
    FSlateApplication::Get().DismissAllMenus();

    if (const USNNodeBase* NodeInstance = Cast<USNNodeBase>(Class->GetDefaultObject()))
    {
        if (!NodeInstance->CanAttachTo(Data->GetClass()))
        {
            const auto& Style = FSNAssetStyleRegistry::Get().GetTrackStyle(Data->GetClass());
            const FString& TypeName = Style->GetTypeName().IsEmpty() ? Data->GetClass()->GetName() : Style->GetTypeName();

            FText SupportMsgs;
            for (const auto* TrackType : NodeInstance->AttachedToTracks())
            {
                const auto& TrackStyle = FSNAssetStyleRegistry::Get().GetTrackStyle(TrackType);
                const FText& TrackTypeName = TrackStyle->GetTypeName().IsEmpty() ? FText::FromString(TrackType->GetName()) : FText::FromString(TrackStyle->GetTypeName());
                SupportMsgs = FText::Format(INVTEXT("{0}\n{1}"), {SupportMsgs, TrackTypeName});
            }

            const FText Msg = FText::Format(INVTEXT("类型\"{0}\"无法添加到轨道\"{1}\"，支持的轨道：\n{2}"), {FText::FromString(Class->GetName()), FText::FromString(TypeName), SupportMsgs});
            FMessageDialog::Open(EAppMsgCategory::Warning, EAppMsgType::Ok, Msg);
            return;
        }
    }

    TObjectPtr<USNNodeBase> Node = Data->MakeNewNode(Class, FName(Class->GetName()));
    // uint32 Frame = TimeRuler->PositionToFrame(FSlateApplication::Get().GetCursorPos().X);
    uint32 Frame = TimeRuler->PositionToFrame(CachedMousePoint.X);
    float Time = TimeRuler->FrameToTime(Frame);
    Node->BeginTime = Time;
    // default use min length
    Node->EndTime = Node->BeginTime + Node->GetMinLength();

    Repaint();
    Context->OnNodeCreated.Broadcast(Node);
    Context->SetSelectNode(Node);
}

void SSNTrackNodes::AddBlueprintNodeMenu(FMenuBuilder& MenuBuilder)
{
    MenuBuilder.AddWidget
    (
        Utils::MakeNewObjectPicker
        (
            USNBlueprintActionNode::StaticClass(),
            ScenarioNarrativeEditor::FOnActionBPClassPicked::CreateRaw(this, &SSNTrackNodes::AddNode),
            ScenarioNarrativeEditor::FOnFilterUClass::CreateLambda([](const UClass* Cls)
            {
                if (!Cls->HasAnyClassFlags(CLASS_CompiledFromBlueprint))
                    return false;
                return true;
            }),
            ScenarioNarrativeEditor::FOnFilterBlueprintData::CreateLambda([this](const TSharedRef<const IUnloadedBlueprintData>& InUnloadedBlueprintData)
            {
                const UClass* Cls = InUnloadedBlueprintData->GetNativeParent();
                if (const USNNodeBase* NodeInstance = Cast<USNNodeBase>(Cls->GetDefaultObject()))
                {
                    const auto Supported = NodeInstance->AttachedToTracks();
                    return NodeInstance->CanAttachTo(Data->GetClass());
                }
                return true;
            })
        ),
        {}
    );
}

void SSNTrackNodes::DeleteNode()
{
    FSlateApplication::Get().DismissAllMenus();
    auto* OperateNode = CachedHoverNode == nullptr ? Context->GetSelectNode() : CachedHoverNode;
    if (Data->RemoveNode(OperateNode))
    {
        if (Context->GetSelectNode() == OperateNode)
        {
            Context->SetSelectNode(nullptr);
        }

        Context->NodeToWidget.Remove(OperateNode);
        Repaint();
        Context->OnNodeDeleted.Broadcast(OperateNode);
        Data->GetOuter()->Modify();
        CachedHoverNode = nullptr;
    }
}

#undef LOCTEXT_NAMESPACE
