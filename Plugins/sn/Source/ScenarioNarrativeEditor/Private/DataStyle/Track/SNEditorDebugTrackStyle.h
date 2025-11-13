// EditorDebugTrackStyle.h
// @author : xiangyp
// @date : 2025/06/23 15:26:45
// @desc :

#pragma once
#include "DataStyle/SNTrackStyle.h"

class SCENARIONARRATIVEEDITOR_API FSNEditorDebugTrackStyle : public FSNTrackStyle
{
public:
    FSNEditorDebugTrackStyle();
    virtual ~FSNEditorDebugTrackStyle() override = default;

    virtual FString GetTypeName() const override;
};
