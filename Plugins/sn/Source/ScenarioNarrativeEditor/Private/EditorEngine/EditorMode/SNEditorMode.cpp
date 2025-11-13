// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorEngine/EditorMode/SNEditorMode.h"
#include "EditorEngine/EditorMode/SNEditorWidgetHelper.h"

const FEditorModeID USNEditorMode::EdModeId = TEXT("SNEditorMode");

USNEditorMode::~USNEditorMode()
{
}


USNEditorMode::USNEditorMode()
{
    Info = FEditorModeInfo(
        USNEditorMode::EdModeId,
        NSLOCTEXT("SNEditorMode", "DisplayName", "Selection"),
        FSlateIcon("EditorStyle", "LevelEditor.SelectMode", "LevelEditor.SelectMode.Small"),
        true, 0);
}

TSharedRef<FLegacyEdModeWidgetHelper> USNEditorMode::CreateWidgetHelper()
{
    return MakeShared<FSNEditorWidgetHelper>();
}

bool USNEditorMode::UsesToolkits() const
{
    return false;
}

bool USNEditorMode::UsesPropertyWidgets() const
{
    return true;
}
