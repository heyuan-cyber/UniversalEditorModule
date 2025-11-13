#include "DataStyle/Node/SNPhysicsCastNodeStyle.h"

#include "ScenarioNarrativeEditorDefines.h"
#include "Node/SNPhysicsCastNode.h"

#define LOCTEXT_NAMESPACE "FSNPhysicsCastNodeStyle"

FSNPhysicsCastNodeStyle::FSNPhysicsCastNodeStyle()
{
    BottomColor = FColor::FromHex("#ff8600");
}

FString FSNPhysicsCastNodeStyle::GetTypeName() const
{
    return TEXT("碰撞检测节点");
}

FText FSNPhysicsCastNodeStyle::GetTooltipText(const TObjectPtr<USNAssetBase>& Asset) const
{
    if (!Asset)
        return {};

    USNPhysicsCastNode* Node = CastChecked<USNPhysicsCastNode>(Asset);
    const UEnum* EnumPtr = StaticEnum<ESNTraceShapeType::Type>();
    return FText::FromName(EnumPtr->GetNameByValue(Node->Shape));
}

void FSNPhysicsCastNodeStyle::OnContextMenu(FMenuBuilder& MenuBuilder, const USNAssetBase* Asset, FSNSequencerContext* Context)
{
    MenuBuilder.AddMenuEntry
    (
        INVTEXT("Bake Collider"),
        INVTEXT("Back preview collider mesh"),
        {},
        FUIAction(FExecuteAction::CreateLambda([this, Asset]
        {
            USNPhysicsCastNode* Node = const_cast<USNPhysicsCastNode*>(Cast<USNPhysicsCastNode>(Asset));
            Node->bIsShowColliderMesh = !Node->bIsShowColliderMesh;
            // UE_LOG(LogScenarioNarrativeEditor, Warning, TEXT("create bake mesh for %s"), *Node->DisplayName.ToString());
        }),
        {},
        FIsActionChecked::CreateLambda([this, Asset]
        {
            const USNPhysicsCastNode* Node = Cast<USNPhysicsCastNode>(Asset);
            return Node->bIsShowColliderMesh;
        })),
        NAME_None,
        EUserInterfaceActionType::ToggleButton
    );
}

#undef LOCTEXT_NAMESPACE
