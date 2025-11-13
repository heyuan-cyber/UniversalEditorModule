// SNTrackStyle.cpp
// @author : xiangyp
// @date : 2025/04/22 16:08:22
// @desc :

#include "DataStyle/SNTrackStyle.h"

#include "SNHotkeys.h"
#include "SequencerEditor/SNTrackHierarchy.h"
#include "Styling/SlateIconFinder.h"
#include "Track/SNTrackBase.h"

#define LOCTEXT_NAMESPACE "SNTrackStyle"

FSNTrackStyle::FSNTrackStyle()
    : TrackEdColor(FLinearColor::Green), TrackIcon(nullptr), DesiredHeight(40.f)
{
    TrackEdColor = FColor::MakeRandomColor();
    TrackIcon = FSlateIconFinder::FindIcon("ClassIcon.DataLayer").GetIcon();
}

void FSNTrackStyle::OnCreateContextMenu(FMenuBuilder& MenuBuilder, const USNAssetBase* Asset, FSNSequencerContext* Context)
{
    OnContextMenu(MenuBuilder, Asset);

    MenuBuilder.BeginSection(FName(TEXT("TrackOperations")), FText::FromString("Operation"));

    // TODO: support undo
    MenuBuilder.AddMenuEntry
    (
        FSNHotkeys::Get().DeleteSNAsset,
        NAME_None,
        FText::Format(INVTEXT("删除 \"{0}\""), FText::FromName(SSNTrackHierarchy::CachedHoverTrack->DisplayName)),
        INVTEXT("删除轨道"),
        FSlateIconFinder::FindIcon("Icons.Delete")
    );

    MenuBuilder.EndSection();
}

#undef LOCTEXT_NAMESPACE
