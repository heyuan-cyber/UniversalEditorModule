#include "MontageSet/SNAnimationClipDataTableRow.h"

#include "MontageSet/SNMontageSetAsset.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"
#include "Widgets/Input/SSpinBox.h"

void SSNAnimationClipDataTableRow::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& OwnerTable)
{
    Item = InArgs._Clip;
    Asset = InArgs._Asset;
    Datas = InArgs._ClipDatas;
    ListView = InArgs._ListView;
    SMultiColumnTableRow::Construct(SMultiColumnTableRow::FArguments(), OwnerTable);
}

TSharedRef<SWidget> SSNAnimationClipDataTableRow::GenerateWidgetForColumn(const FName& ColumnName)
{
    check(Item.IsValid());
    if (ColumnName == "Name")
    {
        return SAssignNew(NameTextBlock, SInlineEditableTextBlock)
            .Text_Lambda([this] { return FText::FromString(Item->Name); })
            .OnVerifyTextChanged_Lambda([this](const FText& InText, FText& ErrorTip)
            {
                if (InText.IsEmptyOrWhitespace())
                {
                    ErrorTip = INVTEXT("不能使用空字符串");
                    return false;
                }

                for (const auto& Data : *Datas)
                {
                    if (Data == Item)
                    {
                        continue;
                    }

                    if (InText.ToString() == Data->Name)
                    {
                        ErrorTip = INVTEXT("该名称已被占用");
                        return false;
                    }
                }
                return true;
            })
            .IsReadOnly(false)
            .OnTextCommitted_Lambda([this](const FText& NewText, ETextCommit::Type CommitType)
            {
                Item->Name = NewText.ToString();
            });
    }

    if (ColumnName == "Begin")
    {
        return SNew(SSpinBox<int>)
            .ToolTipText(FText::FromString(TEXT("起始帧")))
            .Value_Lambda([this] { return Item->BeginFrame; })
            .OnValueChanged_Lambda([this](int Val)
            {
                if (!Item.IsValid())
                {
                    return;
                }

                if (Asset.IsValid() && Asset->Animation)
                {
                    Item->BeginFrame = FMath::Clamp(Val, 0, Item->EndFrame);
                }
                else
                {
                    Item->BeginFrame = FMath::Max(0, Val);
                }
            });
    }

    if (ColumnName == "End")
    {
        return SNew(SSpinBox<int>)
            .ToolTipText(FText::FromString(TEXT("结束帧")))
            .Value_Lambda([this] { return Item->EndFrame; })
            .OnValueChanged_Lambda([this](int Val)
            {
                if (!Item.IsValid())
                {
                    return;
                }

                if (Asset.IsValid() && Asset->Animation)
                {
                    Item->EndFrame = FMath::Clamp(Val, Item->BeginFrame, Asset->Animation->GetNumberOfSampledKeys() - 1);
                }
                else
                {
                    Item->EndFrame = Val;
                }
            });
    }

    if (ColumnName == "Rate")
    {
        return SNew(SSpinBox<float>)
            .ToolTipText(FText::FromString(TEXT("播放速度")))
            .MaxFractionalDigits(2)
            .Value_Lambda([this] { return Item->PlayRate; })
            .OnValueChanged_Lambda([this](float Val)
            {
                if (!Item.IsValid())
                {
                    return;
                }

                Item->PlayRate = FMath::Max(0, Val);
            });
    }

    if (ColumnName == "Operate")
    {
        return SNew(SButton)
            .Text(FText::FromString("-"))
            .ToolTipText(FText::FromString(TEXT("移除该切片")))
            .HAlign(HAlign_Center)
            .OnClicked_Lambda([this]()
            {
                if (!Item.IsValid())
                {
                    return FReply::Handled();
                }

                if (Datas->Remove(Item) > 0)
                {
                    Asset->Clips.Remove(Item.Get());
                    ListView->RequestListRefresh();
                }

                return FReply::Handled();
            });
    }

    return SNullWidget::NullWidget;
}

FReply SSNAnimationClipDataTableRow::OnMouseButtonDoubleClick(const FGeometry& InMyGeometry,
                                                            const FPointerEvent& InMouseEvent)
{
    if (!NameTextBlock.IsValid())
    {
        return SMultiColumnTableRow::OnMouseButtonDoubleClick(InMyGeometry, InMouseEvent);
    }

    // support rename via double click name text block
    FVector2D ScreenPosition = InMouseEvent.GetScreenSpacePosition();
    if (NameTextBlock->GetCachedGeometry().IsUnderLocation(ScreenPosition))
    {
        NameTextBlock->EnterEditingMode();
        return FReply::Handled();
    }

    return SMultiColumnTableRow::OnMouseButtonDoubleClick(InMyGeometry, InMouseEvent);
}
