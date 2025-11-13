#include "DataStyle/DetailCustomization/DialogNodeDetailCustomization.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Node/SNDialogNode.h"
#include "Track/SNDialogTrack.h"
#include "Widgets/Input/SComboBox.h"

TSharedRef<FDialogNodeDetailCustomization> FDialogNodeDetailCustomization::MakeInstance()
{
    return MakeShareable(new FDialogNodeDetailCustomization);
}

void FDialogNodeDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
    // 获取节点/轨道实例
    DetailBuilder.GetObjectsBeingCustomized(CachedObjects);
    USNDialogNode* DialogNode = CachedObjects.Num() > 0 ? Cast<USNDialogNode>(CachedObjects[0].Get()) : nullptr;
    if (!DialogNode) return;

    USNDialogTrack* Track = Cast<USNDialogTrack>(DialogNode->Parent);
    if(!Track) return;

    // 获取下拉框数据
    ComboItems.Empty();
    for (auto& Data : Track->DialogData)
    {
        ComboItems.Add(MakeShared<FString>(FString::FromInt(Data.NodeId)));
    }


    if(ComboItems.Num() == 0) InitialSelection = MakeShared<FString>(TEXT("请选择对话CSV文件"));
    else if(DialogNode->NodeIndex == -1) InitialSelection = MakeShared<FString>(UTF8_TO_TCHAR("请选择对话Id"));
    else InitialSelection = ComboItems[DialogNode->NodeIndex];

    // 在细节面板添加自定义行
    IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("Settings");
    Category.AddCustomRow(FText::FromString("Dynamic Combo"))
        .NameContent()
        [
            SNew(STextBlock).Text(FText::FromString(UTF8_TO_TCHAR("对话Id")))
        ]
        .ValueContent()
        [
            SAssignNew(ComboBox, SComboBox<TSharedPtr<FString>>)
            .OptionsSource(&ComboItems)
            .OnSelectionChanged_Lambda([DialogNode, Track, this](TSharedPtr<FString> NewValue, ESelectInfo::Type)
            {

                if (NewValue.IsValid())
                {
                    // 更新节点变量：查找选项索引
                    FString NodeIdString = *NewValue;
                    const int32 NewNodeId = FCString::Atoi(*NodeIdString);
                    const int32 NewNodeIndex = ComboItems.IndexOfByKey(NewValue);
                    if (NewNodeIndex != INDEX_NONE)
                    {
                        DialogNode->NodeIndex = NewNodeIndex;
                        DialogNode->NodeData = Track->DialogData[NewNodeIndex];
                        DialogNode->Modify(true); // 标记为脏，支持撤销
                    }
                }
            })
            .OnGenerateWidget_Lambda([](TSharedPtr<FString> Item)
            {
                return SNew(STextBlock).Text(FText::FromString(*Item));
            })
            .InitiallySelectedItem(InitialSelection)
            [
                SNew(STextBlock)
                    .Text_Lambda([this]() -> FText {
                    return FText::FromString(*InitialSelection);})
            ]
        ];
}
