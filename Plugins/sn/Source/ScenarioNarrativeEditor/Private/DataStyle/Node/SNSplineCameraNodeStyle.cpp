#include "DataStyle/Node/SNSplineCameraNodeStyle.h"

#include "ScenarioNarrativeEditorDefines.h"
#include "JsonObjectConverter.h"
#include "Node/SNCameraNode.h"
#include "Node/SNSplineCameraNode.h"
#include "SequencerEditor/SNNodeWidget.h"
#include "Serialize/JsonSerialization.h"

#define LOCTEXT_NAMESPACE "FSNCameraNodeStyle"

FSNSplineCameraNodeStyle::FSNSplineCameraNodeStyle()
{
    BottomColor = FLinearColor::FromSRGBColor(FColor(80, 123, 72));
}

FString FSNSplineCameraNodeStyle::GetTypeName() const
{
    return TEXT("轨道相机节点");
}

FText FSNSplineCameraNodeStyle::GetTooltipText(const TObjectPtr<USNAssetBase>& Asset) const
{
    if (!Asset)
    {
        return {};
    }

    return FText::GetEmpty();
}

void FSNSplineCameraNodeStyle::OnContextMenu(FMenuBuilder& MenuBuilder, const USNAssetBase* Asset, FSNSequencerContext* Context)
{
    MenuBuilder.AddMenuEntry(INVTEXT("ToJson"), INVTEXT(""), {}, FExecuteAction::CreateLambda([Asset]
    {
        const USNSplineCameraNode* AnimAsset = Cast<USNSplineCameraNode>(Asset);
        FString JsonStr;
        FSNNodeBlend Blend;
        Blend.CustomCurve = NewObject<UCurveFloat>();
        FJsonObjectConverter::UStructToJsonObjectString(Blend, JsonStr);

        UE_LOG(LogScenarioNarrativeEditor, Display, TEXT("%s"), *JsonStr);
    }));
    MenuBuilder.AddMenuEntry(INVTEXT("添加关键帧"), INVTEXT(""), {}, FExecuteAction::CreateLambda([Asset, Context]
    {
        const USNSplineCameraNode* CameraNode = Cast<USNSplineCameraNode>(Asset);
        FSNSplineCameraKeyFrame NewKeyFrame;
        NewKeyFrame.Time = 0.0f;

        CameraNode->KeyFrames.Add(NewKeyFrame);
        Context->NodeToWidget[CameraNode]->Repaint();
    }));
}

#undef LOCTEXT_NAMESPACE
