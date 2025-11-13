#include "DataStyle/Node/SNAnimationNodeStyle.h"

#include "ScenarioNarrativeEditorDefines.h"
#include "JsonObjectConverter.h"
#include "Node/SNAnimationNode.h"
#include "Serialize/JsonSerialization.h"

#define LOCTEXT_NAMESPACE "FSNAnimationNodeStyle"

FSNAnimationNodeStyle::FSNAnimationNodeStyle()
{
    BottomColor = FLinearColor::FromSRGBColor(FColor(80, 123, 72));
}

FString FSNAnimationNodeStyle::GetTypeName() const
{
    return TEXT("动画节点");
}

FText FSNAnimationNodeStyle::GetTooltipText(const TObjectPtr<USNAssetBase>& Asset) const
{
    if (!Asset)
    {
        return {};
    }

    if (const auto& AnimAsset = Cast<USNAnimationNode>(Asset))
    {
        return AnimAsset->Animation ? FText::FromString(AnimAsset->Animation->GetName()) : FText::GetEmpty();
    }

    UE_LOG(LogScenarioNarrativeEditor, Error, TEXT("Something was wrong... %s mapped to AnimationNodeStyle "),
           *Asset->GetClass()->GetName());
    return FText::GetEmpty();
}

void FSNAnimationNodeStyle::OnContextMenu(FMenuBuilder& MenuBuilder, const USNAssetBase* Asset, FSNSequencerContext* Context)
{
    MenuBuilder.AddMenuEntry(INVTEXT("ToJson"), INVTEXT(""), {}, FExecuteAction::CreateLambda([Asset]
    {
        const USNAnimationNode* AnimAsset = Cast<USNAnimationNode>(Asset);
        FString JsonStr;
        // FJsonObjectConverter::UStructToJsonObjectString(AnimAsset->BlendIn, JsonStr);
        FSNNodeBlend Blend;
        Blend.CustomCurve = NewObject<UCurveFloat>();
        FJsonObjectConverter::UStructToJsonObjectString(Blend, JsonStr);

        // FString Result;
        // const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Result);
        // FJsonSerializer::Serialize(FJsonHelper::ToJsonObject(AnimAsset).ToSharedRef(), Writer);

        UE_LOG(LogScenarioNarrativeEditor, Display, TEXT("%s"), *JsonStr);
    }));
}

#undef LOCTEXT_NAMESPACE
