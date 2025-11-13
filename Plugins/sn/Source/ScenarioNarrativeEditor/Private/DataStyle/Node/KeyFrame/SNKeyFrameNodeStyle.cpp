#include "DataStyle/Node/KeyFrame/SNKeyFrameNodeStyle.h"
#include "ScenarioNarrativeEditorDefines.h"
#include "JsonObjectConverter.h"
#include "SNNodeBlend.h"
#include "Node/SNNodeBase.h"
#include "SequencerEditor/SNNodeWidget.h"
#include "Node/KeyFrame/SNKeyFrame.h"
#include "Preview/SNAssetPreviewPerformer.h"
#include "Preview/SNPreviewManager.h"
#include "Styling/SlateIconFinder.h"
#include "Preview/SNPreviewContext.h"
#include "Widget/SNTimeRulerWidget.h"

#define LOCTEXT_NAMESPACE "FSNKeyFrameNodeStyle"

FSNKeyFrameNodeStyle::FSNKeyFrameNodeStyle()
{
    BottomColor = FLinearColor::FromSRGBColor(FColor(80, 123, 72));
    BackgroundColor = FLinearColor::Gray;
    Icon = FSlateIconFinder::FindIcon("Icons.Sword");
    TypeName = TEXT("关键帧节点");
    SNSequencerContext = nullptr;
}

FSNKeyFrameNodeStyle::FSNKeyFrameNodeStyle(FString InTypeName, FLinearColor InBottomColor, FLinearColor InBackgroundColor, FSlateIcon InIcon)
{
    BottomColor = InBottomColor;
    BackgroundColor = InBackgroundColor;
    Icon = InIcon;
    TypeName = InTypeName;
    SNSequencerContext = nullptr;
}
FSNKeyFrameNodeStyle::~FSNKeyFrameNodeStyle()
{
    SNSequencerContext = nullptr;
}
void FSNKeyFrameNodeStyle::OnContextMenu(FMenuBuilder& MenuBuilder, const USNAssetBase* Asset, FSNSequencerContext* InContext)
{
    if (!SNSequencerContext) SNSequencerContext = InContext;
    MenuBuilder.AddMenuEntry(INVTEXT("ToJson"), INVTEXT(""), {}, FExecuteAction::CreateLambda([Asset]
    {
        FString JsonStr;
        FSNNodeBlend Blend;
        Blend.CustomCurve = NewObject<UCurveFloat>();
        FJsonObjectConverter::UStructToJsonObjectString(Blend, JsonStr);

        UE_LOG(LogScenarioNarrativeEditor, Display, TEXT("%s"), *JsonStr);
    }));

    // 判断 Asset 是否实现了接口
    if (Asset && Asset->GetClass()->ImplementsInterface(USNKeyFrameInterface::StaticClass()))
    {
            // 添加关键帧操作
            MenuBuilder.AddMenuEntry(
                INVTEXT("添加关键帧"),
                INVTEXT("在当前时间位置添加关键帧"),
                FSlateIcon(),
                FExecuteAction::CreateLambda([this, Asset]()
                {
                    SNSequencerContext->PreviewManager.Get()->PreviewContext.Get()->Performer->CreateAndShowKeyFrame(const_cast<USNNodeBase*>(Cast<USNNodeBase>(Asset)), this);
                }
            ));
    }
}
int32 FSNKeyFrameNodeStyle::HasSelectKeyFrame(const USNNodeBase* Node)
{
    if (!SNSequencerContext) return -1;
    UObject* object = SNSequencerContext->GetCurrentSelectedUObject();
    if (object != nullptr && IsValid(object))
    {
        USNKeyFrameWrapper* KeyFrameWrapper = Cast<USNKeyFrameWrapper>(object);
        if(KeyFrameWrapper && Node == KeyFrameWrapper->SourceNode)
            return KeyFrameWrapper->KeyFrameId;
    }
    return -1;
}

void FSNKeyFrameNodeStyle::ShowAddedKeyFrame(USNKeyFrameWrapper* Wrapper)
{
    if (!SNSequencerContext) return;
    SNSequencerContext->SetSelectUObject(Wrapper);
    SNSequencerContext->NodeToWidget[Wrapper->SourceNode.Get()]->Repaint();
}

void FSNKeyFrameNodeStyle::OnKeyFrameClicked(USNKeyFrameWrapper* Wrapper)
{
    if (!Wrapper || !SNSequencerContext || !SNSequencerContext->TimeRulerWidget)
    {
        return;
    }

    // 获取时间轴 widget
    SSNTimeRulerWidget* TimeRuler = SNSequencerContext->TimeRulerWidget;

    // 将关键帧的时间转换为滑条位置
    float KeyTime = Wrapper->SourceNode->BeginTime + Wrapper->KeyFrame.LocalTime;

    // 设置滑条到关键帧时间
    TimeRuler->CommitValue(KeyTime);
}

#undef LOCTEXT_NAMESPACE
