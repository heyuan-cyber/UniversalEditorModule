// SNNodeStyle.cpp
// @author : xiangyp
// @date : 2025/04/22 16:03:07
// @desc :

#include "DataStyle/SNNodeStyle.h"

#include "SNHotkeys.h"
#include "Node/SNNodeBase.h"
#include "SequencerEditor/SNTrackNodes.h"
#include "Styling/SlateIconFinder.h"

#define LOCTEXT_NAMESPACE "SNNodeStyle"

FSNNodeStyle::FSNNodeStyle()
    : BackgroundColor(FLinearColor::Gray), BottomColor(FColor::Transparent)
{
    BottomColor = FColor::MakeRandomColor();
    Icon = FSlateIconFinder::FindIcon("Icons.Sword");
}

void FSNNodeStyle::OnCreateContextMenu(FMenuBuilder& MenuBuilder, const USNAssetBase* Asset, FSNSequencerContext* Context)
{
    OnContextMenu(MenuBuilder, Asset, Context);

    MenuBuilder.BeginSection(FName(TEXT("NodeOperations")), FText::FromString("Operation"));
    // TODO: support undo

    // copy & paste

    // cut

    // delete
    MenuBuilder.AddMenuEntry
    (
        FSNHotkeys::Get().DeleteSNAsset,
        NAME_None,
        FText::Format(INVTEXT("删除 \"{0}\""), FText::FromName(SSNTrackNodes::CachedHoverNode->DisplayName)),
        INVTEXT("删除节点"),
        FSlateIconFinder::FindIcon("Icons.Delete")
    );

    MenuBuilder.EndSection();
}

#undef LOCTEXT_NAMESPACE
