#include "MontageSet/SNMontageSetClipWidget.h"

#include "ObjectTools.h"
#include "PropertyCustomizationHelpers.h"
#include "Animation/AnimSequenceHelpers.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "MontageSet/SNAnimationClipDataTableRow.h"
#include "MontageSet/SNMontageSetAsset.h"
#include "UObject/SavePackage.h"

#define LOCTEXT_NAMESPACE "SSNMontageSetClipWidget"

void SSNMontageSetClipWidget::Construct(const FArguments& InArgs)
{
    SetAsset(InArgs._Asset.Get());
    OnAnimationChanged = InArgs._OnAnimationChanged;
    OnSelctedClipChanged = InArgs._OnSelectedClipChanged;

    FMargin DefaultPadding(20, 2);
    ChildSlot
    [
        SNew(SVerticalBox)
        + SVerticalBox::Slot()
        .Padding(DefaultPadding)
        .AutoHeight()
        [
            SNew(SVerticalBox)
            + SVerticalBox::Slot()
            [
                SNew(STextBlock)
                .Text_Lambda([this]
                {
                    if (Asset.IsValid())
                    {
                        return FText::Format(INVTEXT("Asset: {0}"), FText::FromString(Asset->GetName()));
                    }
                    return INVTEXT("No Asset Selected!");
                })
            ]
            + SVerticalBox::Slot()
            [
                SNew(STextBlock)
                .Text_Lambda([this]
                {
                    if (Asset->Animation)
                    {
                        return FText::Format(INVTEXT("Animation: {0}"), FText::FromString(Asset->Animation->GetName()));
                    }
                    return INVTEXT("No Animation Selected!");
                })
            ]
            + SVerticalBox::Slot()
            [
                MakeAnimationAssetSelectorWidget()
            ]
        ]
        + SVerticalBox::Slot()
        .Padding(DefaultPadding)
        .AutoHeight()
        [
            MakeAssetControlWidget()
        ]
        + SVerticalBox::Slot()
        .Padding(DefaultPadding)
        .FillHeight(1.f)
        [
            MakeClipListView()
        ]
        + SVerticalBox::Slot()
        .Padding(DefaultPadding)
        .AutoHeight()
        [
            SNew(STextBlock)
            .Text_Lambda([this]()
            {
                if (!Asset.IsValid())
                {
                    return FText::FromString("No Asset Selected");
                }
                return FText::FromString(FString::Format(TEXT("Clips Sum: {0}"), {Asset->Clips.Num()}));
            })
        ]
    ];
}

void SSNMontageSetClipWidget::RequestListRefresh() const
{
    if (!ClipsListView.IsValid())
    {
        return;
    }

    ClipsListView->RequestListRefresh();
}

void SSNMontageSetClipWidget::SetAsset(USNMontageSetAsset* InAsset)
{
    if (Asset == InAsset)
    {
        return;
    }

    Asset = InAsset;
    ClipDatas = GetClips();
    // ClipsListView->SetSelection(ClipDatas.IsEmpty() ? nullptr : ClipDatas.Top());
    CurrentSelected = nullptr;

    if (OnAnimationChanged.IsBound())
    {
        OnAnimationChanged.Execute(InAsset->Animation);
    }
}

TArray<FSNClipDataPtr> SSNMontageSetClipWidget::GetClips() const
{
    TArray<TWeakObjectPtr<USNAnimationClipData>> Clips;
    if (!Asset.IsValid())
    {
        return Clips;
    }

    Clips.Reserve(Asset->Clips.Num());
    for (const auto& Item : Asset->Clips)
    {
        Clips.Add(Item);
    }

    return Clips;
}

TSharedRef<ITableRow> SSNMontageSetClipWidget::GenerateClipRow(FSNClipDataPtr Item,
                                                             const TSharedRef<STableViewBase>& Owner)
{
    return SNew(SSNAnimationClipDataTableRow, Owner)
        .Clip(Item)
        .Asset(Asset)
        .ListView(ClipsListView)
        .ClipDatas(&ClipDatas);
}

TSharedRef<SHorizontalBox> SSNMontageSetClipWidget::MakeAnimationAssetSelectorWidget() const
{
    return SNew(SHorizontalBox)
        + SHorizontalBox::Slot()
        .AutoWidth()
        .Padding(FMargin(5, 0))
        .VAlign(VAlign_Center)
        [
            SNew(STextBlock)
            .Text(FText::FromString("Animation"))
        ]
        + SHorizontalBox::Slot()
        .VAlign(VAlign_Center)
        [
            // TODO: find out why DisplayThumbnail settings not work even its true
            SNew(SObjectPropertyEntryBox)
            .AllowedClass(UAnimSequence::StaticClass())
            .DisplayThumbnail(false)
            .AllowClear(true)
            .AllowCreate(false)
            .OnObjectChanged_Lambda([this](const FAssetData& InAssetData)
            {
                if (!Asset.IsValid())
                {
                    return;
                }

                if (Asset->Animation == InAssetData.GetAsset())
                {
                    return;
                }

                Asset->Animation = Cast<UAnimSequence>(InAssetData.GetAsset());
                if (OnAnimationChanged.IsBound())
                {
                    OnAnimationChanged.Execute(Asset->Animation);
                }
                UE_LOG(LogScenarioNarrativeEditor, Log, TEXT("animation changed to %s"),
                       (Asset->Animation ? *(Asset->Animation->GetName()) : TEXT("nullptr")));
            })
            .ObjectPath_Lambda([this]()
            {
                if (!Asset.IsValid() || !Asset->Animation)
                {
                    return FString();
                }

                return Asset->Animation.GetPath();
            })
        ];
}

TSharedRef<SVerticalBox> SSNMontageSetClipWidget::MakeAssetControlWidget()
{
    return SNew(SVerticalBox)
        + SVerticalBox::Slot()
        .AutoHeight()
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            .FillWidth(3.f)
            [
                SNew(SButton)
                .Text(FText::FromString(TEXT("Add Clip")))
                .HAlign(HAlign_Center)
                .OnClicked_Lambda([this]()
                {
                    if (!Asset.IsValid())
                    {
                        return FReply::Unhandled();
                    }

                    ClipDatas.Add(Asset->CreateNewClip());
                    ClipsListView->RequestListRefresh();
                    ClipsListView->ScrollToBottom();

                    ClipsListView->SetSelection(ClipDatas.Last());
                    return FReply::Handled();
                })
            ]

            + SHorizontalBox::Slot()
            .FillWidth(1.f)
            [
                SNew(SButton)
                .Text(FText::FromString(TEXT("Generate Animation")))
                .HAlign(HAlign_Center)
                .OnClicked_Lambda([this]()
                {
                    if (!Asset.IsValid() || ClipDatas.IsEmpty())
                    {
                        return FReply::Unhandled();
                    }

                    for (const auto& Clip : ClipDatas)
                    {
                        GenerateClipSequence(Clip);
                    }
                    return FReply::Handled();
                })
            ]
        ];
}

TSharedRef<SListView<FSNClipDataPtr>> SSNMontageSetClipWidget::MakeClipListView()
{
    SAssignNew(ClipsListView, SListView<FSNClipDataPtr>)
    .HeaderRow
    (
        SNew(SHeaderRow)
        + SHeaderRow::Column("Name")
        .DefaultLabel(FText::FromString("Clip Name"))
        .FillWidth(0.3f)
        + SHeaderRow::Column("Begin")
        .DefaultLabel(FText::FromString("Start Frame"))
        .FillWidth(0.2f)
        + SHeaderRow::Column("End")
        .DefaultLabel(FText::FromString("End Frame"))
        .FillWidth(0.2f)
        + SHeaderRow::Column("Rate")
        .DefaultLabel(FText::FromString("Play Rate"))
        .FillWidth(0.2f)
        + SHeaderRow::Column("Operate")
        .DefaultLabel(FText::FromString("Operate"))
        .FillWidth(0.1f)
    )
    .SelectionMode(ESelectionMode::Type::Single)
    .ListItemsSource(&ClipDatas)
    .OnSelectionChanged_Lambda([this](FSNClipDataPtr NewSelect, ESelectInfo::Type Type)
    {
        CurrentSelected = NewSelect;
        if (OnSelctedClipChanged.IsBound())
        {
            OnSelctedClipChanged.Execute(CurrentSelected);
        }
    })
    .OnGenerateRow(this, &SSNMontageSetClipWidget::GenerateClipRow);

    return ClipsListView.ToSharedRef();
}

void SSNMontageSetClipWidget::GenerateClipSequence(const FSNClipDataPtr& Clip) const
{
    if (!Asset->Animation)
    {
        FMessageDialog::Open(EAppMsgType::Ok, INVTEXT("未指定动画，无法生成切片"));
        return;
    }

    // 尝试把已有的文件和切片关联起来
    TryRelink();

    UAnimSequenceBase* SourceSequence = Asset->Animation.Get();
    if (SourceSequence && !Clip->Name.IsEmpty())
    {
        const FString ClipName = SourceSequence->GetOutermost()->GetName();
        FString PackagePath = FPackageName::GetLongPackagePath(ClipName);
        const FString CheckName = Clip->Name + "_Clip";
        // 检查目标路径是否已存在同名文件
        const FString TargetPath = FString::Printf(TEXT("%s/%s.%s"), *PackagePath, *CheckName, *CheckName);

        if (StaticLoadObject(UAnimSequence::StaticClass(), nullptr, *TargetPath))
        {
            // 如果已存在同名文件，则跳过
            UE_LOG(LogTemp, Log, TEXT("跳过已存在的Clip资源: %s"), *TargetPath);
            return;
        }

        if (Clip->Asset && Clip->Asset != SourceSequence)
        {
            // 删除旧Asset
            UAnimationAsset* DeleteAsset = Clip->Asset;

            GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->CloseAllEditorsForAsset(DeleteAsset);
            if (DeleteAsset && IsValid(DeleteAsset))
            {
                TArray<UObject*> ToDeleteObjects = {DeleteAsset};
                ObjectTools::ForceDeleteObjects(ToDeleteObjects, false);
            }
            Clip->Asset = nullptr;
        }

        const FString DuplicateName = (Clip->Name) + "_Clip";
        UPackage* Package = CreatePackage(*(PackagePath / DuplicateName));
        UObject* NewObj = StaticDuplicateObject(SourceSequence, Package, *DuplicateName);

        if (UAnimSequence* NewSequence = Cast<UAnimSequence>(NewObj))
        {
            IAnimationDataController& Controller = NewSequence->GetController();
            Controller.OpenBracket(FText::FromString("Trim Animation"));
            NewSequence->Modify();

            auto Trim = [&](int32 Start, int32 Num)
            {
                if (Num > 0)
                {
                    IAnimationDataController::FScopedBracket ScopedBracket(Controller
                                                                           , LOCTEXT("TrimRawAnimation_Bracket", "Trimming Animation Track Data"));
                    UE::Anim::AnimationData::RemoveKeys(NewSequence, Start, Num);
                }
            };

            int32 SequenceFrame = NewSequence->GetNumberOfSampledKeys();
            Trim(0, Clip->BeginFrame);
            Trim(Clip->EndFrame - Clip->BeginFrame, SequenceFrame - Clip->EndFrame);

            Controller.CloseBracket();

            FAssetRegistryModule::AssetCreated(NewSequence);
            Package->MarkPackageDirty();

            const FString FilePath = FPackageName::LongPackageNameToFilename(PackagePath
                                                                             , FPackageName::GetAssetPackageExtension());
            FSavePackageArgs SaveArgs;
            SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
            SaveArgs.SaveFlags = SAVE_NoError;
            UPackage::SavePackage(Package, nullptr, *FilePath, SaveArgs);

            Clip->Asset = NewSequence;
        }
    }
}

void SSNMontageSetClipWidget::TryRelink() const
{
    if (!Asset->Animation)
        return;

    const auto& Module = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
    const auto& AssetRegistry = Module.Get();
    // const FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));

    const FString ClipName = Asset->Animation->GetOutermost()->GetName();
    for (const auto& Clip : Asset->Clips)
    {
        if (Clip->Asset)
            continue;

        FString PackagePath = FPackageName::GetLongPackagePath(ClipName) / Clip->Name + "_Clip";
        // const FString FilePath = FPackageName::LongPackageNameToFilename(PackagePath, FPackageName::GetAssetPackageExtension());
        if (AssetRegistry.DoesPackageExistOnDisk(FName(PackagePath)))
        {
            Clip->Asset = Cast<UAnimationAsset>(StaticLoadAsset(UAnimationAsset::StaticClass(), FTopLevelAssetPath(PackagePath)));
        }
    }
}

// void SMontageSetClipWidget::TrimSequences(UAnimSequence* AnimationSequence, int32 StartFrame, int32 EndFrame)
// {
//     // TODO: 裁剪
// }

#undef LOCTEXT_NAMESPACE
