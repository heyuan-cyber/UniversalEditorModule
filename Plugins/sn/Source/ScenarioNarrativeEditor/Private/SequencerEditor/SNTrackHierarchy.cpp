#include "SequencerEditor/SNTrackHierarchy.h"

#include "SNHotkeys.h"
#include "SNSequencerContext.h"
#include "ScenarioNarrativeEditorDefines.h"
#include "Widget/SNTimeRulerWidget.h"
#include "Utils.h"
#include "DataStyle/SNAssetStyleRegistry.h"
#include "DataStyle/SNTrackStyle.h"
#include "Node/SNNodeBase.h"
#include "Preview/SNPreviewManager.h"
#include "Sequencer/SNSequencer.h"
#include "SequencerEditor/SNTrackWidget.h"
#include "Track/SNTrackBase.h"

#define LOCTEXT_NAMESPACE "SNTrackHierarchy"

USNTrackBase* SSNTrackHierarchy::CachedHoverTrack = nullptr;

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSNTrackHierarchy::Construct(const FArguments& InArgs)
{
    Data = InArgs._Asset;
    Context = InArgs._Context;

    ChildSlot
    [
        SNew(SBox)
        .VAlign(VAlign_Top)
        [
            SNew(SVerticalBox)
            + SVerticalBox::Slot()
            .AutoHeight()
            [
                SNew(SBox)
                .HeightOverride(30)
                [
                    SNew(SHorizontalBox)
                    + SHorizontalBox::Slot()
                    .HAlign(HAlign_Center)
                    .FillWidth(0.7f)
                    [
                        // button area
                        SNew(SHorizontalBox)
                        + SHorizontalBox::Slot()
                        .AutoWidth()
                        [
                            SNew(SButton)
                            .Text(INVTEXT("Add Track"))
                            .VAlign(VAlign_Center)
                            .OnClicked_Lambda([this]
                            {
                                OpenTrackCreateMenu(FWidgetPath(), FSlateApplication::Get().GetCursorPos());
                                return FReply::Handled();
                            })
                        ]
                    ]
                    + SHorizontalBox::Slot()
                    .VAlign(VAlign_Center)
                    .FillWidth(0.3f)
                    [
                        SNew(STextBlock)
                        .Text_Lambda([this]
                        {
                            FNumberFormattingOptions NFO;
                            NFO.MinimumFractionalDigits = 2;
                            NFO.MaximumFractionalDigits = 2;

                            float MaxLength = Data->GetMaxLength();
                            FFormatOrderedArguments Args;
                            Args.Add(FText::AsNumber(Context->TimeRulerWidget->TimeToFrame(Context->PreviewManager->CurrentTime)));
                            Args.Add(FText::AsNumber(Context->TimeRulerWidget->TimeToFrame(MaxLength)));
                            Args.Add(FText::AsNumber(Context->PreviewManager->CurrentTime, &NFO));
                            Args.Add(FText::AsNumber(MaxLength, &NFO));
                            return FText::Format(INVTEXT("{0}/{1} ({2}s/{3}s)"), Args);
                        })
                        .Justification(ETextJustify::Type::Center)
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
                SAssignNew(Layout, SScrollBox)
                .Orientation(Orient_Vertical)
                .ScrollBarAlwaysVisible(false)
                .ScrollBarVisibility(EVisibility::Collapsed)
                .OnUserScrolled(InArgs._OnUserScrolled)
            ]
        ]
    ];

    BindCommands();
    Repaint();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

SSNTrackHierarchy::~SSNTrackHierarchy()
{
    // UnbindCommands?
}

FReply SSNTrackHierarchy::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    CachedHoverTrack = nullptr;
    if (IsHovered() && MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
    {
        CachedHoverTrack = Context->CurrentHoveredTrack;
        FWidgetPath WidgetPath = MouseEvent.GetEventPath() != nullptr ? *MouseEvent.GetEventPath() : FWidgetPath();
        OpenTrackCreateMenu(WidgetPath, MouseEvent.GetScreenSpacePosition());
        return FReply::Handled();
    }

    if (IsDirectlyHovered() && MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        Context->SetSelectTrack(nullptr);
        return FReply::Handled();
    }

    return SCompoundWidget::OnMouseButtonDown(MyGeometry, MouseEvent);
}

void SSNTrackHierarchy::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    SCompoundWidget::OnMouseEnter(MyGeometry, MouseEvent);
    // refresh some command binding's SN
    BindCommands();
}

void SSNTrackHierarchy::Repaint()
{
    Layout->ClearChildren();
    Context->TrackToWidget.Empty();

    for (const auto& Track : Data->Tracks)
    {
        if (!Track)
        {
            Layout->AddSlot().Padding(FMargin(0, 2))[SNew(SBox).HeightOverride(40)
            [
                SNew(STextBlock).Text(INVTEXT("ERROR TRACK!!!"))
            ]];
            continue;
        }

        TSharedPtr<SSNTrackWidget> TrackWidget;
        Layout->AddSlot().Padding(FMargin(0, 2))
        [
            SAssignNew(TrackWidget, SSNTrackWidget)
            .TrackData(Track)
            .Context(Context)
        ];

        Context->TrackToWidget.Add(Track, TrackWidget.Get());
    }

    Context->TrackHierarchyWidget = this;
}

void SSNTrackHierarchy::SetScrollOffset(float Offset) const
{
    if (!Layout.IsValid() || Layout->GetScrollOffset() == Offset)
    {
        return;
    }

    Layout->SetScrollOffset(Offset);
}

void SSNTrackHierarchy::OpenTrackCreateMenu(const FWidgetPath& InOwnerPath, const UE::Slate::FDeprecateVector2DParameter& SummonLocation)
{
    FSlateApplication::Get().PushMenu
    (
        AsShared(),
        InOwnerPath,
        OnCreteContextMenu(),
        SummonLocation,
        FPopupTransitionEffect(FPopupTransitionEffect::ContextMenu)
    );
}

TSharedRef<SWidget> SSNTrackHierarchy::OnCreteContextMenu()
{
    FMenuBuilder MenuBuilder(true, Context->ToolkitCommands);
    MenuBuilder.AddSearchWidget();

    if (Context->CurrentHoveredTrack)
    {
        auto* Track = Context->CurrentHoveredTrack;
        if (auto* Style = FSNAssetStyleRegistry::Get().GetTrackStyle(Track->GetClass()))
        {
            Style->OnCreateContextMenu(MenuBuilder, Track, Context.Get());
        }
    }

    auto Classes = Utils::GetUObjectDerivedClasses<USNTrackBase>();
    if (!Classes.IsEmpty())
    {
        MenuBuilder.BeginSection(FName(TEXT("AddSNTracks")), FText::FromString("Add Track"));
        // TODO: support catalog via submenu
        for (const auto& Cls : Classes)
        {
            const auto& Style = FSNAssetStyleRegistry::Get().GetTrackStyle(Cls);
            const FString& TypeName = Style->GetTypeName().IsEmpty() ? Cls->GetName() : Style->GetTypeName();
            MenuBuilder.AddMenuEntry
            (
                FText::FromString(TypeName),
                Style->GetTooltipText(nullptr),
                FSlateIcon(),
                FUIAction(FExecuteAction::CreateLambda([this, Cls]
                {
                    TObjectPtr<USNTrackBase> NewTrack = Data->MakeNewTrack(Cls, FName());
                    Repaint();
                    Context->OnTrackCreated.Broadcast(NewTrack);
                    Context->SetSelectTrack(NewTrack);
                }))
            );
        }
        MenuBuilder.EndSection();
    }

    return MenuBuilder.MakeWidget();
}

void SSNTrackHierarchy::DeleteTrack()
{
    auto* OperateNode = CachedHoverTrack == nullptr ? Context->GetSelectTrack() : CachedHoverTrack;
    if (!OperateNode)
        return;

    const FScopedTransaction DelTransSN(INVTEXT("删除轨道"));
    Data->Modify();

    for (const auto& Node : OperateNode->Nodes)
    {
        if (!Node)
            continue;

        if (Context->GetSelectNode() == Node)
        {
            Context->SetSelectNode(nullptr);
        }

        Node->Parent = nullptr;
        Context->NodeToWidget.Remove(Node);
        Context->OnNodeDeleted.Broadcast(Node);
    }

    if (Data->Tracks.Remove(OperateNode))
    {
        Context->SetSelectTrack(nullptr);
        Repaint();
        Context->OnTrackDeleted.Broadcast(OperateNode);
        // Data->Modify();
    }
}

void SSNTrackHierarchy::BindCommands()
{
    Context->ToolkitCommands->MapAction(FSNHotkeys::Get().DeleteSNAsset, FExecuteAction::CreateSP(this, &SSNTrackHierarchy::DeleteTrack));
}

#undef LOCTEXT_NAMESPACE
