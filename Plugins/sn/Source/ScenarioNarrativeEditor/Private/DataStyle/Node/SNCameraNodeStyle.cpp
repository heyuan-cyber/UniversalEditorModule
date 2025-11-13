#include "DataStyle/Node/SNCameraNodeStyle.h"

#include "ScenarioNarrativeEditorDefines.h"
#include "JsonObjectConverter.h"
#include "CustomDrawer/SNCustomActionDrawerBase.h"
#include "Node/SNCameraNode.h"
#include "SequencerEditor/SNNodeWidget.h"
#include "Serialize/JsonSerialization.h"
#include "CustomDrawer/SNCustomActionDrawerRegistry.h"
#include "Preview/SNPreviewManager.h"

#define LOCTEXT_NAMESPACE "FSNCameraNodeStyle"

FSNCameraNodeStyle::FSNCameraNodeStyle()
{
    BottomColor = FLinearColor::FromSRGBColor(FColor(80, 123, 72));
}

FString FSNCameraNodeStyle::GetTypeName() const
{
    return TEXT("相机节点");
}

FText FSNCameraNodeStyle::GetTooltipText(const TObjectPtr<USNAssetBase>& Asset) const
{
    if (!Asset)
    {
        return {};
    }

    return FText::GetEmpty();
}

void FSNCameraNodeStyle::OnContextMenu(FMenuBuilder& MenuBuilder, const USNAssetBase* Asset, FSNSequencerContext* Context)
{
    MenuBuilder.AddMenuEntry(INVTEXT("ToJson"), INVTEXT(""), {}, FExecuteAction::CreateLambda([Asset]
    {
        const USNCameraNode* AnimAsset = Cast<USNCameraNode>(Asset);
        FString JsonStr;
        FSNNodeBlend Blend;
        Blend.CustomCurve = NewObject<UCurveFloat>();
        FJsonObjectConverter::UStructToJsonObjectString(Blend, JsonStr);

        UE_LOG(LogScenarioNarrativeEditor, Display, TEXT("%s"), *JsonStr);
    }));
    MenuBuilder.AddMenuEntry(INVTEXT("添加关键帧"), INVTEXT(""), {}, FExecuteAction::CreateLambda([Asset, Context]
    {
        const USNCameraNode* CameraNode = Cast<USNCameraNode>(Asset);
        FSNCameraKeyFrame NewKeyFrame;
        NewKeyFrame.Time = Context->PreviewManager->CurrentTime - CameraNode->BeginTime;
        NewKeyFrame.FOV = 0.0f;

        // 找到插入位置
        int32 InsertIndex = 0;
        for (; InsertIndex < CameraNode->KeyFrames.Num(); ++InsertIndex)
        {
            if (CameraNode->KeyFrames[InsertIndex].Time > NewKeyFrame.Time)
            {
                break;
            }
        }
        CameraNode->KeyFrames.Insert(NewKeyFrame, InsertIndex);

        // 选中新增的关键帧
        auto* Drawer = FSNCustomActionDrawerRegistry::Get().GetDrawer(CameraNode);
        USNCameraKeyFrameWrapper* Wrapper = Drawer->CreateTransientObject<USNCameraKeyFrameWrapper>();
        Wrapper->CameraKeyFrame = NewKeyFrame;
        Wrapper->KeyFrameId = InsertIndex;
        Wrapper->SourceNode = const_cast<USNCameraNode*>(CameraNode);
        Context->SetSelectUObject(Wrapper);

        Context->NodeToWidget[CameraNode]->Repaint();
    }));
}

#undef LOCTEXT_NAMESPACE
